#pragma once

#include <parlay/sequence.h>

#include <random>

namespace NAlgoLab::NUtils {
    parlay::sequence<int> GenerateRandomParlaySeq(size_t n);
}
