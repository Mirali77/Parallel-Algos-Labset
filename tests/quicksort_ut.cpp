#include <gtest/gtest.h>

#include "../src/quicksort.h"
#include "../src/utils.h"

TEST(QuickSortTest, ParallelQuickSortNoBatchingTest) {
    size_t N = 1e6;
    auto data = NAlgoLab::NUtils::GenerateRandomParlaySeq(N);
    NAlgoLab::NQuickSort::parallel_quicksort(data, /* batch_threshold */ 1);
    for (int i = 1; i < N; i++) {
        ASSERT_TRUE(data[i - 1] <= data[i]);
    }
}

TEST(QuickSortTest, ParallelQuickSortWithBatchingTest) {
    size_t N = 1e6;
    auto data = NAlgoLab::NUtils::GenerateRandomParlaySeq(N);
    NAlgoLab::NQuickSort::parallel_quicksort(data, /* batch_threshold */ 1e3);
    for (int i = 1; i < N; i++) {
        ASSERT_TRUE(data[i - 1] <= data[i]);
    }
}

TEST(QuickSortTest, SequentialQuickSortTest) {
    size_t N = 1e6;
    auto data = NAlgoLab::NUtils::GenerateRandomParlaySeq(N);
    NAlgoLab::NQuickSort::seq_quicksort(data);
    for (int i = 1; i < N; i++) {
        ASSERT_TRUE(data[i - 1] <= data[i]);
    }
}
