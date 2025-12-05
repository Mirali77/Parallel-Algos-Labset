#include "utils.h"

#include <random>

namespace NAlgoLab::NUtils {
    parlay::sequence<int> GenerateRandomParlaySeq(size_t n, int values_lowerbound, int values_upperbound) {
        parlay::sequence<int> data(n);
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> dist(values_lowerbound, values_upperbound);
        for (auto& it : data) it = dist(gen);
        return data;
    }
}