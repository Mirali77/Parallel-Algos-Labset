#include "quicksort.h"
#include "utils.h"

#include <iostream>
#include <chrono>
#include <algorithm>

namespace NAlgoLab::NQuickSort {
    void quicksort_benchmark(size_t n, size_t attempts_count, size_t batch_treshhold) {
        parlay::sequence<int> data = NAlgoLab::NUtils::GenerateRandomParlaySeq(n);
        std::cout << "Data initialized" << std::endl << std::endl;

        double par_ms_avg = 0;
        double seq_ms_avg = 0;
        for (size_t i = 1; i <= attempts_count; i++) {
            std::cout << "Attempt " << i << ":" << std::endl;
            auto arr = data;
            // for (auto& it: arr) std::cout << it << " "; std::cout << std::endl;
            auto t0 = std::chrono::high_resolution_clock::now();
            parallel_quicksort(arr, batch_treshhold);
            auto t1 = std::chrono::high_resolution_clock::now();
            // for (auto& it: arr) std::cout << it << " "; std::cout << std::endl;
            auto par_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            std::cout << "Parallel quicksort: " << par_ms << "ms" << std::endl;

            arr = data;
            // for (auto& it: arr) std::cout << it << " "; std::cout << std::endl;
            t0 = std::chrono::high_resolution_clock::now();
            seq_quicksort(arr);
            t1 = std::chrono::high_resolution_clock::now();
            // for (auto& it: arr) std::cout << it << " "; std::cout << std::endl;
            auto seq_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            std::cout << "Sequential sort: " << seq_ms << "ms" << std::endl << std::endl;

            par_ms_avg += par_ms;
            seq_ms_avg += seq_ms;
        }

        par_ms_avg /= attempts_count;
        seq_ms_avg /= attempts_count;

        std::cout << "Parallel quicksort average: " << std::setprecision(4) << std::fixed << par_ms_avg << "ms" << std::endl;
        std::cout << "Sequential sort average: " << std::setprecision(4) << std::fixed << seq_ms_avg << "ms" << std::endl;
    }
}