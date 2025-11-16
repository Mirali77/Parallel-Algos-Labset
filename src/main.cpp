#include <iostream>
#include "quicksort.h"

int main() {
    NAlgoLab::NQuickSort::quicksort_benchmark(1e8, 5, 1 << 18);
    return 0;
}
