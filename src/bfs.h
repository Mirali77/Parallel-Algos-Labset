#pragma once

#include "utils.h"

#include <parlay/sequence.h>

namespace NAlgoLab::NBfs {
    parlay::sequence<int> seq_bfs(
        int start_vertex,
        const NUtils::TGraph& adj_list
    ); // -> distances from start vertex to others

    parlay::sequence<int> parallel_bfs(
        int start_vertex,
        const NUtils::TGraph& adj_list,
        int front_batch_treshold,
        int block_sz
    ); // -> distances from start vertex to others

    void bfs_cube_graph_benchmark(size_t n, size_t attempts_count, int front_batch_treshold, int block_sz);
}
