#pragma once

#include <random>

#include <parlay/primitives.h>
#include <parlay/sequence.h>
#include <parlay/slice.h>
#include <parlay/parallel.h>

namespace {
    // Стандартное извлечение пивота для последовательного квиксорта
    template <typename T>
    size_t lomuto_partition(parlay::slice<T*, T*> s) {
        size_t n = s.size();
        T pivot = s[n - 1];
        size_t i = 0;
        for (size_t j = 0; j + 1 < n; ++j) {
            if (s[j] <= pivot) {
                std::swap(s[i], s[j]);
                ++i;
            }
        }
        std::swap(s[i], s[n - 1]);
        return i;
    }

    template <class T>
    inline T median3pivot(T& a, T& b, T& c) {
        T& ab_min = (a < b) ? a : b;
        T& ab_max = (a < b) ? b : a;
        return c < ab_min ? ab_min : c > ab_max ? ab_max : c;
    }
}

namespace NAlgoLab::NQuickSort {
    template <typename T>
    void seq_quicksort(parlay::slice<T*, T*> in) {
        size_t n = in.size();

        if (n <= 1) return;

        auto p = lomuto_partition(in);

        seq_quicksort(in.cut(0, p));
        seq_quicksort(in.cut(p + 1, n));
    }

    // Достаточно дешёвое извлечение пивота (дешевле рандомного)
    template <typename T>
    void seq_quicksort(parlay::sequence<T>& in) {
        size_t n = in.size();
        seq_quicksort(in.cut(0, n));
    }

    template <class T>
    static std::pair<size_t, size_t> block_partition(
        parlay::slice<T*, T*> in,
        parlay::slice<T*, T*> buf,
        T& pivot,
        size_t block_size = 1 << 12
    ) { // -> <количество элементов меньше пивота, количество элементов равных пивоту>
        size_t n = in.size();
        if (n == 0) return {0, 0};
        size_t block_n = (n + block_size - 1) / block_size;
        parlay::sequence<size_t> less_blocks(block_n), equal_blocks(block_n), more_blocks(block_n);

        // Количества в блоках
        parlay::parallel_for(0, block_n, [&](size_t b) {
            auto start = b * block_size;
            auto end = std::min(n, start + block_size);
            size_t lc = 0, ec = 0;
            for (size_t i = start; i < end; ++i) {
                lc += (in[i] < pivot);
                ec += (in[i] == pivot);
            }
            less_blocks[b] = lc;
            equal_blocks[b] = ec;
            more_blocks[b]  = (end - start) - lc - ec;
        });

        // Блочные сканы
        auto less_pair   = parlay::scan(less_blocks);
        auto equal_pair  = parlay::scan(equal_blocks);
        auto more_pair   = parlay::scan(more_blocks);

        parlay::sequence<size_t>& less_pref  = less_pair.first;
        parlay::sequence<size_t>& equal_pref = equal_pair.first;
        parlay::sequence<size_t>& more_pref  = more_pair.first;

        size_t less_count  = less_pair.second;
        size_t equal_count = equal_pair.second;
        size_t more_count  = more_pair.second;

        // Перекладываем из значения в buf с помощью блоков
        parlay::parallel_for(0, block_n, [&](size_t b) {
            size_t start = b * block_size;
            size_t end = std::min(n, start + block_size);

            size_t less_common = less_pref[b];
            size_t leq_common = less_count + equal_pref[b];
            size_t more_common = less_count + equal_count + more_pref[b];

            size_t lp = 0, ep = 0, mp = 0;
            for (size_t i = start; i < end; ++i) {
                T& x = in[i];
                if (x < pivot) {
                    buf[less_common + lp++] = x;
                } else if (x == pivot) {
                    buf[leq_common + ep++] = x;
                } else {
                    buf[more_common + mp++] = x;
                }
            }
        });

        return {less_count, equal_count};
    }

    template <typename T>
    void parallel_quicksort(
        parlay::slice<T*, T*> in,
        parlay::slice<T*, T*> buf, // Работаем с in и buf поочерёдно, чтобы не алоцировать лишние массивы
        size_t batch_threshold, // Лимит, при достижении размера батча которого переходим к последовательному алгосу
        bool to_in // Параметр, по которому контролируем местонахождение ответа в in или buf
    ) {
        size_t n = in.size();
        if (n <= batch_threshold) {
            if (to_in) {
                std::sort(in.begin(), in.end());
            } else {
                parlay::parallel_for(0, n, [&](size_t i){ buf[i] = in[i]; });
                std::sort(buf.begin(), buf.begin() + n);
            }
            return;
        }

        T pivot = median3pivot(in[0], in[n/2], in[n-1]);

        // Расставляем элементы по buf-у относительнр пивота с блочной оптимизацией
        auto [less_count, equal_count] = block_partition(in, buf, pivot);

        auto left_in   = parlay::make_slice(buf.cut(0, less_count));
        auto left_buf  = parlay::make_slice(in.cut(0, less_count));
        auto right_in  = parlay::make_slice(buf.cut(less_count + equal_count, n));
        auto right_buf = parlay::make_slice(in.cut(less_count + equal_count, n));

        parlay::par_do(
            [&]{
                parallel_quicksort(left_in, left_buf, batch_threshold, !to_in);
            },
            [&]{
                parallel_quicksort(right_in, right_buf, batch_threshold, !to_in);
            }
        );

        // Так как в подотрезок равных элементов не спускаемся - важно его скопировать, если ответ в buf-е
        if (to_in && equal_count > 0) {
            parlay::parallel_for(0, equal_count, [&](size_t i){
                in[less_count + i] = buf[less_count + i];
            });
        }
    }

    template <typename T>
    void parallel_quicksort(parlay::sequence<T>& in, size_t batch_threshold) {
        size_t n = in.size();
        auto buf = parlay::sequence<T>(n);
        parallel_quicksort(in.cut(0, n), buf.cut(0, n), batch_threshold, true);
    }

    void quicksort_benchmark(size_t n, size_t attempts_count = 5, size_t batch_threshold = 1024);
}