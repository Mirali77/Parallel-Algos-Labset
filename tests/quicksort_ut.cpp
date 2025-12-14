#include <gtest/gtest.h>

#include "../src/quicksort.h"
#include "../src/utils.h"

TEST(QuickSortTest, ParallelQuickSortNoBatchingRandomDataTest) {
    size_t N = 1e6;
    auto data = NAlgoLab::NUtils::generate_random_parlay_seq(N);
    NAlgoLab::NQuickSort::parallel_quicksort(data, /* batch_threshold */ 1);
    for (int i = 1; i < N; i++) {
        ASSERT_TRUE(data[i - 1] <= data[i])
            << "Элемент "
            << data[i - 1]
            << " c индексом "
            << i - 1
            << " оказался больше следующего элемента "
            << data[i];
    }
}

TEST(QuickSortTest, ParallelQuickSortWithBatchingRandomDataTest) {
    size_t N = 1e6;
    auto data = NAlgoLab::NUtils::generate_random_parlay_seq(N);
    NAlgoLab::NQuickSort::parallel_quicksort(data, /* batch_threshold */ 1e3);
    for (int i = 1; i < N; i++) {
        ASSERT_TRUE(data[i - 1] <= data[i])
            << "Элемент "
            << data[i - 1]
            << " c индексом "
            << i - 1
            << " оказался больше следующего элемента "
            << data[i];
    }
}

TEST(QuickSortTest, SequentialQuickSortRandomDataTest) {
    size_t N = 1e6;
    auto data = NAlgoLab::NUtils::generate_random_parlay_seq(N);
    NAlgoLab::NQuickSort::seq_quicksort(data);
    for (int i = 1; i < N; i++) {
        ASSERT_TRUE(data[i - 1] <= data[i])
            << "Элемент "
            << data[i - 1]
            << " c индексом "
            << i - 1
            << " оказался больше следующего элемента "
            << data[i];
    }
}

TEST(QuickSortTest, ParallelQuickSortLowVarietyRandomDataTest) {
    size_t N = 1e6;
    // Значения лежат на отрезке [0, 7]
    // Особенность: много равных элементов
    auto data = NAlgoLab::NUtils::generate_random_parlay_seq(N, /* values lowerbound */ 0, /* values upperbound */ 7);

    auto timer_start = std::chrono::high_resolution_clock::now();
    NAlgoLab::NQuickSort::parallel_quicksort(data, /* batch_threshold */ 1e3);
    auto timer_end = std::chrono::high_resolution_clock::now();

    {
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - timer_start).count();
        ASSERT_LT(duration_ms, 1000) << "Сортировка заняла больше 1 сек";
    }

    for (int i = 1; i < N; i++) {
        ASSERT_TRUE(data[i - 1] <= data[i])
            << "Элемент "
            << data[i - 1]
            << " c индексом "
            << i - 1
            << " оказался больше следующего элемента "
            << data[i];
    }
    for (int i = 0; i < N; i++) {
        ASSERT_TRUE(0 <= data[i] && data[i] <= 7)
            << "Элемент "
            << data[i]
            << "с индексом "
            << i
            << "оказался вне отрезка [0, 7]";
    }
}

TEST(QuickSortTest, ParallelQuickSortAllEqualDataTest) {
    size_t N = 1e6;
    // Массив из семёрок
    auto data = NAlgoLab::NUtils::generate_random_parlay_seq(N, 7, 7);

    NAlgoLab::NQuickSort::parallel_quicksort(data, /* batch_threshold */ 1e3);

    for (int i = 1; i < N; i++) {
        ASSERT_TRUE(data[i - 1] == data[i])
            << "Элемент "
            << data[i - 1]
            << " c индексом "
            << i - 1
            << " оказался не равен следующему элементу "
            << data[i];
    }
}

TEST(QuickSortTest, ParallelQuickSortSortedDataTest) {
    size_t N = 1e6;
    // Массив уже изначально отсортирован
    auto data = NAlgoLab::NUtils::generate_random_parlay_seq(N);
    std::sort(data.begin(), data.end());

    auto timer_start = std::chrono::high_resolution_clock::now();
    NAlgoLab::NQuickSort::parallel_quicksort(data, /* batch_threshold */ 1e3);
    auto timer_end = std::chrono::high_resolution_clock::now();

    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - timer_start).count();
    ASSERT_LT(duration_ms, 1000) << "Сортировка заняла больше 1 сек: " << duration_ms << "мс";
}

TEST(QuickSortTest, ParallelQuickSortReversedSortedDataTest) {
    size_t N = 1e6;
    // Массив изначально отсортирован в обратном порядке
    auto data = NAlgoLab::NUtils::generate_random_parlay_seq(N);
    std::sort(data.rbegin(), data.rend());
    ASSERT_TRUE(data[0] > data[N - 1]);

    auto timer_start = std::chrono::high_resolution_clock::now();
    NAlgoLab::NQuickSort::parallel_quicksort(data, /* batch_threshold */ 1e3);
    auto timer_end = std::chrono::high_resolution_clock::now();

    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - timer_start).count();
    ASSERT_LT(duration_ms, 1000) << "Сортировка заняла больше 1 сек: " << duration_ms << "мс";
}

TEST(QuickSortTest, ParallelQuickSortConstantValuesTest) {
    size_t N = 1e6;
    // Массив захардкожен. Проверка на то, что элементы принимают прежние значения.
    auto data = parlay::sequence<int>{5, 7, 2, 8, 1, 3, 2};

    NAlgoLab::NQuickSort::parallel_quicksort(data, /* batch_threshold */ 1e3);

    auto expected_data = parlay::sequence<int>{1, 2, 2, 3, 5, 7, 8};

    ASSERT_EQ(data, expected_data);
}
