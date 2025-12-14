#include "utils.h"

#include <random>
#include <parlay/parallel.h>
#include <parlay/primitives.h>

namespace NAlgoLab::NUtils {
    parlay::sequence<int> generate_random_parlay_seq(size_t n, int values_lowerbound, int values_upperbound) {
        parlay::sequence<int> data(n);
        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> dist(values_lowerbound, values_upperbound);
        for (auto& it : data) it = dist(gen);
        return data;
    }

    TGraph::TGraph(int n) {
        AdjList.resize(n);
    }
    
    void TGraph::add_edge(int from, int to) {
        assert(from >= 0 && from < AdjList.size());
        assert(to >= 0 && to < AdjList.size());
        AdjList[from].push_back(to);
    }

    std::tuple<int, int, int> get_coord(int v, int n) {
        int x = v / (n * n);
        int y = (int)(v / n) % n;
        int z = v % n;
        return {x, y, z};
    }

    TGraph generate_cube_graph(int n) {
        auto N = n * n * n;
        TGraph graph(N);
        parlay::parallel_for(0, N, [&](int idx) {
            auto [x, y, z] = get_coord(idx, n);
            int cnt = 0;
            for (int dx = -1; dx <= 1; dx++) for (int dy = -1; dy <= 1; dy++) for (int dz = -1; dz <= 1; dz++) {
                if (
                    abs(dx) + abs(dy) + abs(dz) != 1
                    || x + dx < 0 || x + dx >= n
                    || y + dy < 0 || y + dy >= n
                    || z + dz < 0 || z + dz >= n
                ) continue;
                auto next = (x + dx) * n * n + (y + dy) * n + (z + dz);
                graph.add_edge(idx, next);
            }
        });
        return graph;
    }
}
