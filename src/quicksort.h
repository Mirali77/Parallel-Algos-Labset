#pragma once

#include <random>

#include <parlay/primitives.h>
#include <parlay/sequence.h>
#include <parlay/slice.h>
#include <parlay/parallel.h>

namespace {
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

    template <typename T>
    void seq_quicksort(parlay::sequence<T>& in) {
        size_t n = in.size();
        seq_quicksort(in.cut(0, n));
    }

    template <typename T>
    void parallel_quicksort(parlay::slice<T*, T*> in, parlay::slice<T*, T*> buf, size_t batch_treshhold, int depth = 0) {
        size_t n = in.size();
        if (n <= batch_treshhold || depth > 5) {
            seq_quicksort(in);
            return;
        }
        
        auto pivot = [&]() {
            std::mt19937 gen(std::random_device{}());
            return in[gen() % n];
        }();

        auto less  = parlay::tabulate(n, [&](size_t i) -> int { return in[i] < pivot ? 1 : 0; });
        auto equal = parlay::tabulate(n, [&](size_t i) -> int { return in[i] == pivot ? 1 : 0; });
        auto more  = parlay::tabulate(n, [&](size_t i) -> int { return in[i] > pivot ? 1 : 0; });

        parlay::sequence<T> less_scan  = parlay::scan(less).first;
        parlay::sequence<T> equal_scan = parlay::scan(equal).first;
        parlay::sequence<T> more_scan  = parlay::scan(more).first;

        size_t less_count  = less_scan[n-1] + less[n-1];
        size_t equal_count = equal_scan[n-1] + equal[n-1];

        parlay::parallel_for(0, n, [&](size_t i) {
            if (in[i] < pivot)
                buf[less_scan[i]] = in[i];
            else if (in[i] == pivot)
                buf[less_count + equal_scan[i]] = in[i];
            else
                buf[less_count + equal_count + more_scan[i]] = in[i];
        });

        parlay::par_do(
            [&](){
                parallel_quicksort(parlay::make_slice(buf.cut(0, less_count)), parlay::make_slice(in.cut(0, less_count)), batch_treshhold, depth + 1);
            },
            [&](){
                parallel_quicksort(
                    parlay::make_slice(buf.cut(less_count + equal_count, n)),
                    parlay::make_slice(in.cut(less_count + equal_count, n)),
                    batch_treshhold,
                    depth + 1
                );
            }
        );
        
        parlay::parallel_for(0, n, [&](size_t i) {
            in[i] = buf[i];
        });
    }

    template <typename T>
    void parallel_quicksort(parlay::sequence<T>& in, size_t batch_treshhold) {
        size_t n = in.size();
        auto buf = parlay::sequence<T>(n);
        parallel_quicksort(in.cut(0, n), buf.cut(0, n), batch_treshhold);
    }

    void quicksort_benchmark(size_t n, size_t attempts_count = 5, size_t batch_treshhold = 1024);
}