#include <gtest/gtest.h>

#include "../src/bfs.h"
#include "../src/utils.h"

TEST(BfsTest, CubeGraph) {
    int n = 100;
    int N = n * n * n;
    auto graph = NAlgoLab::NUtils::generate_cube_graph(n);

    auto run_and_check = [&](int start_x, int start_y, int start_z) {
        int start_v = start_x * n * n + start_y * n + start_z;

        auto seq_result = NAlgoLab::NBfs::seq_bfs(start_v, graph);
        auto par_result = NAlgoLab::NBfs::parallel_bfs(start_v, graph, 0, n / 10);

        for (int v = 0; v < N; v++) {
            auto [x, y, z] = NAlgoLab::NUtils::get_coord(v, n);
            auto dist = abs(x - start_x) + abs(y - start_y) + abs(z - start_z);

            ASSERT_EQ(seq_result[v], dist)
                << "Вершина v: "
                << x << " " << y << " " << z
                << " , стартовая вершина: "
                << start_x << " " << start_y << " " << start_z
                << ", упало на последовательной";
            ASSERT_EQ(par_result[v], dist)
                << "Вершина v: "
                << x << " " << y << " " << z
                << " , стартовая вершина: "
                << start_x << " " << start_y << " " << start_z
                << ", упало на параллельной";
        }
    };

    run_and_check(0, 0, 0);
    run_and_check(n - 1, n - 1, n - 1);
    
    std::mt19937 gen(std::random_device{}());
    for (auto _ = 0; _ < 10; _++) {
        auto rand_x = gen() % n;
        auto rand_y = gen() % n;
        auto rand_z = gen() % n;
        run_and_check(rand_x, rand_y, rand_z);
    }
}

TEST(BfsTest, PathGraph) {
    int n = 1e5;
    NAlgoLab::NUtils::TGraph graph(n);
    for (int i = 0; i < n - 1; i++) {
        graph.add_edge(i, i + 1);
        graph.add_edge(i + 1, i);
    }

    auto run_and_check = [&](int start_v) {
        auto seq_result = NAlgoLab::NBfs::seq_bfs(start_v, graph);
        auto par_result = NAlgoLab::NBfs::parallel_bfs(start_v, graph, 0, n / 1000);

        for (int v = 0; v < n; v++) {
            ASSERT_EQ(seq_result[v], abs(v - start_v)) << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на последовательной";
            ASSERT_EQ(par_result[v], abs(v - start_v)) << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на параллельной";
        }
    };

    run_and_check(0);
    run_and_check(n - 1);

    std::mt19937 gen(std::random_device{}());
    for (auto _ = 0; _ < 10; _++) {
        run_and_check(gen() % n);
    }
}

