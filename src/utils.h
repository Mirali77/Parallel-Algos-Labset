#pragma once

#include <parlay/sequence.h>

#include <random>

namespace NAlgoLab::NUtils {
    parlay::sequence<int> GenerateRandomParlaySeq(
        size_t n,
        int values_lowerbound = (int)-1e9,
        int values_upperbound = (int)1e9
    );
}
