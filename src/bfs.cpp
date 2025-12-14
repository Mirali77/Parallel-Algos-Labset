#include "bfs.h"

#include <parlay/parallel.h>
#include <parlay/primitives.h>

#include <atomic>
#include <iostream>
#include <chrono>

namespace NAlgoLab::NBfs {
    parlay::sequence<int> seq_bfs(int start_vertex, const NUtils::TGraph& graph) {
        int N = graph.AdjList.size();
        parlay::sequence<int> distances(N, -1);

        std::queue<int> q;
        q.push(start_vertex);
        distances[start_vertex] = 0;

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto next : graph.AdjList[v]) {
                if (distances[next] == -1) {
                    distances[next] = distances[v] + 1;
                    q.push(next);
                }
            }
        }

        return distances;
    }

    parlay::sequence<int> parallel_bfs(
        int start_vertex,
        const NUtils::TGraph& graph,
        int front_batch_treshold,
        int block_sz
    ) {
        int N = graph.AdjList.size();
        auto distances = parlay::tabulate(N, [](size_t idx) {
            return std::atomic<int>(-1);
        });

        std::vector<int> layer;

        layer.push_back(start_vertex);
        distances[start_vertex] = 0;
        int next_distance = 0;

        while (!layer.empty()) {
            next_distance++;
            int m = layer.size();

            // SEQUENTIONAL
            if (m < front_batch_treshold) {
                std::vector<int> next_layer;
                for(auto& v : layer) {
                    for (auto next : graph.AdjList[v]) {
                        if (auto& dst = distances[next]; dst.load(std::memory_order_relaxed) == -1) {
                            dst.store(next_distance, std::memory_order_relaxed);
                            next_layer.push_back(next);
                        }
                    }
                }
                std::swap(layer, next_layer);
                continue;
            }
            
            // PARALLEL
            int blocks_cnt = (m + block_sz - 1) / block_sz;

            std::vector<std::vector<int>> local_next_layers(blocks_cnt);
            parlay::sequence<int> local_next_layers_size(blocks_cnt);

            parlay::parallel_for(0, blocks_cnt, [&](int block_idx) {
                int begin = block_idx * block_sz;
                int end = std::min(m, (block_idx + 1) * block_sz);
                auto& local_next_layer = local_next_layers[block_idx];
                
                int edges_cnt = 0;
                for (int v_idx = begin; v_idx < end; v_idx++) {
                    edges_cnt += graph.AdjList[layer[v_idx]].size();
                }

                local_next_layer.reserve(edges_cnt / 2 + 20);
                auto& local_next_layer_size = local_next_layers_size[block_idx];

                for (int v_idx = begin; v_idx < end; v_idx++) {
                    auto& v = layer[v_idx];
                    for (auto& next : graph.AdjList[v]) {
                        if (auto cas_expected = -1; distances[next].compare_exchange_strong(
                            cas_expected,
                            next_distance,
                            std::memory_order_relaxed
                        )) {
                            local_next_layer.push_back(next);
                            local_next_layer_size++;
                        }
                    }
                }
            });

            auto [offset, total_count] = parlay::scan(std::move(local_next_layers_size));
            layer.resize(total_count);

            parlay::parallel_for(0, blocks_cnt, [&local_next_layers, &layer, &offset](int block_idx) {
                auto& local_next_layer = local_next_layers[block_idx];
                std::copy(local_next_layer.begin(), local_next_layer.end(), layer.begin() + offset[block_idx]);
            });
        }

        return parlay::tabulate(N, [&distances](size_t idx) { return distances[idx].load(); });
    }

    void bfs_cube_graph_benchmark(size_t n, size_t attempts_count, int front_batch_treshold, int block_sz) {
        auto graph = NUtils::generate_cube_graph(n);
        std::cout << "Data initialized" << std::endl << std::endl;

        double par_ms_avg = 0;
        double seq_ms_avg = 0;
        for (size_t i = 1; i <= attempts_count; i++) {
            std::cout << "Attempt " << i << ":" << std::endl;

            auto t0 = std::chrono::high_resolution_clock::now();
            auto dist = parallel_bfs(0, graph, front_batch_treshold, block_sz);
            auto t1 = std::chrono::high_resolution_clock::now();
            auto par_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            std::cout << "Parallel bfs: " << par_ms << "ms" << std::endl;

            t0 = std::chrono::high_resolution_clock::now();
            dist = seq_bfs(0, graph);
            t1 = std::chrono::high_resolution_clock::now();
            auto seq_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            std::cout << "Sequential bfs: " << seq_ms << "ms" << std::endl << std::endl;

            par_ms_avg += par_ms;
            seq_ms_avg += seq_ms;
        }

        par_ms_avg /= attempts_count;
        seq_ms_avg /= attempts_count;

        std::cout << "Parallel bfs average: " << std::setprecision(4) << std::fixed << par_ms_avg << "ms" << std::endl;
        std::cout << "Sequential bfs average: " << std::setprecision(4) << std::fixed << seq_ms_avg << "ms" << std::endl;
    }
}