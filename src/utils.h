#pragma once

#include <parlay/sequence.h>

#include <random>

namespace NAlgoLab::NUtils {
    parlay::sequence<int> generate_random_parlay_seq(
        size_t n,
        int values_lowerbound = (int)-1e9,
        int values_upperbound = (int)1e9
    );

    struct TGraph {
        TGraph(int n = 0);

        void add_edge(int from, int to);

        std::vector<std::vector<int>> AdjList;
    };

    std::tuple<int, int, int> get_coord(int v, int n);

    TGraph generate_cube_graph(int n);
}
