#include "utils.h"

#include <random>

namespace NAlgoLab::NUtils {
    parlay::sequence<int> GenerateRandomParlaySeq(size_t n) {
        parlay::sequence<int> data(n);
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> dist((int)-1e9, (int)1e9);
        for (auto& it : data) it = dist(gen);
        return data;
    }
}