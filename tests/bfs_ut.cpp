#include <gtest/gtest.h>

#include "../src/bfs.h"
#include "../src/utils.h"

TEST(BfsTest, CubeGraphTest) {
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

            EXPECT_EQ(seq_result[v], dist)
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

TEST(BfsTest, PathGraphTest) {
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
            EXPECT_EQ(seq_result[v], abs(v - start_v)) << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на последовательной";
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

TEST(BfsTest, DirectedCycleGraphTest) {
    int n = 1e5;
    NAlgoLab::NUtils::TGraph graph(n);
    for (int i = 0; i < n; i++) {
        auto next = (i + 1) % n;
        graph.add_edge(i, next);
    }

    auto run_and_check = [&](int start_v) {
        auto seq_result = NAlgoLab::NBfs::seq_bfs(start_v, graph);
        auto par_result = NAlgoLab::NBfs::parallel_bfs(start_v, graph, 0, n / 1000);

        for (int v = 0; v < n; v++) {
            auto expected_dist = (v - start_v + n) % n;

            EXPECT_EQ(seq_result[v], expected_dist) << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на последовательной";
            ASSERT_EQ(par_result[v], expected_dist) << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на параллельной";
        }
    };

    run_and_check(0);
    run_and_check(n - 1);

    std::mt19937 gen(std::random_device{}());
    for (auto _ = 0; _ < 10; _++) {
        run_and_check(gen() % n);
    }
}

TEST(BfsTest, UndirectedCycleGraphTest) {
    int n = 1e5;
    NAlgoLab::NUtils::TGraph graph(n);
    for (int i = 0; i < n; i++) {
        auto next = (i + 1) % n;
        graph.add_edge(i, next);
        graph.add_edge(next, i);
    }

    auto run_and_check = [&](int start_v) {
        auto seq_result = NAlgoLab::NBfs::seq_bfs(start_v, graph);
        auto par_result = NAlgoLab::NBfs::parallel_bfs(start_v, graph, 0, n / 1000);

        for (int v = 0; v < n; v++) {
            auto expected_dist = std::min((v - start_v + n) % n, (start_v - v + n) % n);

            EXPECT_EQ(seq_result[v], expected_dist) << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на последовательной";
            ASSERT_EQ(par_result[v], expected_dist) << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на параллельной";
        }
    };

    run_and_check(0);
    run_and_check(n - 1);

    std::mt19937 gen(std::random_device{}());
    for (auto _ = 0; _ < 10; _++) {
        run_and_check(gen() % n);
    }
}

TEST(BfsTest, CompleteGraphTest) {
    int n = 1000;
    NAlgoLab::NUtils::TGraph graph(n);
    for(int v1 = 0; v1 < n; v1++) for(int v2 = v1 + 1; v2 < n; v2++) {
        graph.add_edge(v1, v2);
        graph.add_edge(v2, v1);
    }

    auto run_and_check = [&](int start_v) {
        auto seq_result = NAlgoLab::NBfs::seq_bfs(start_v, graph);
        auto par_result = NAlgoLab::NBfs::parallel_bfs(start_v, graph, 0, n / 1000);

        for (int v = 0; v < n; v++) {
            EXPECT_EQ(seq_result[v], v == start_v ? 0 : 1) << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на последовательной";
            ASSERT_EQ(par_result[v], v == start_v ? 0 : 1) << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на параллельной";
        }
    };

    std::mt19937 gen(std::random_device{}());
    for (auto _ = 0; _ < 50; _++) {
        run_and_check(gen() % n);
    }
}

TEST(BfsTest, BinaryTreeGraphTest) {
    int n = 1e4;
    NAlgoLab::NUtils::TGraph graph(n);

    for (int i = 1; i < n; ++i) {
        int parent = (i - 1) >> 1;
        graph.add_edge(parent, i);
        graph.add_edge(i, parent);
    }

    auto dist = [&](int a, int b) {
        std::map<int, int> a_anc_dist;
        {
            int dist = 0;
            while (true) {
                a_anc_dist[a] = dist++;
                if (a > 0) a = (a - 1) >> 1;
                else break;
            }
        }
        auto it = a_anc_dist.find(b);
        int dist = 0;
        while (it == a_anc_dist.end()) {
            b = (b - 1) >> 1;
            it = a_anc_dist.find(b);
            dist++;
        }
        return dist + it->second;
    };

    auto run_and_check = [&](int start_v) {
        auto seq_result = NAlgoLab::NBfs::seq_bfs(start_v, graph);
        auto par_result = NAlgoLab::NBfs::parallel_bfs(start_v, graph, 0, n / 1000);

        for (int v = 0; v < n; ++v) {
            EXPECT_EQ(seq_result[v], dist(v, start_v))
                << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на последовательной";
            ASSERT_EQ(par_result[v], dist(v, start_v))
                << "Вершина v: " << v << ", стартовая вершина: " << start_v << ", упало на параллельной";
        }
    };

    run_and_check(0);
    run_and_check(n-1);

    std::mt19937 gen(std::random_device{}());
    for (auto _ = 0; _ < 10; _++) {
        run_and_check(gen() % n);
    }
}

TEST(BfsTest, MultipleEdgesAndSelfLoopsGraphTest) {
    auto n = 5;
    NAlgoLab::NUtils::TGraph graph(n);
    for (auto [from, to] : std::vector<std::pair<int, int>>{
        {0, 0},
        {0, 1},
        {0, 1},
        {1, 1},
        {1, 3},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 4},
        {2, 1}
    }) {
        graph.add_edge(from, to);
    }
    auto start_v = 0;
    // 0-0: 0 = 0
    // 0-1: 0 -> 1 = 1
    // 0-2: 0 -> 1 -> 2 = 2
    // 0-3: 0 -> 1 -> 3 = 2
    // 0-4: 0 -> 1 -> 3 -> 4 = 3
    parlay::sequence<int> expected_distances{0, 1, 2, 2, 3};

    EXPECT_EQ(NAlgoLab::NBfs::seq_bfs(start_v, graph), expected_distances)
        << "Упала последовательная версия";
    ASSERT_EQ(NAlgoLab::NBfs::parallel_bfs(start_v, graph, 0, 1), expected_distances)
        << "Упала параллельная версия";
}

TEST(BfsTest, NoEdgesGraphTest) {
    auto n = 100;
    NAlgoLab::NUtils::TGraph graph(n);

    for (auto start_v = 0; start_v < n; start_v++) {
        parlay::sequence<int> expected_distances(n, -1);
        expected_distances[start_v] = 0;

        EXPECT_EQ(NAlgoLab::NBfs::seq_bfs(start_v, graph), expected_distances)
            << "Упала последовательная версия на стартовой вершине " << start_v;
        ASSERT_EQ(NAlgoLab::NBfs::parallel_bfs(start_v, graph, 0, 10), expected_distances)
            << "Упала параллельная версия на стартовой вершине " << start_v;
    }
}

TEST(BfsTest, RandomSparseGraphTestParallelOnly) {
    // Здесь мы доверяем последовательному BFS
    auto n = 100;
    auto m = 3 * n;
    NAlgoLab::NUtils::TGraph graph(n);

    std::mt19937 gen(std::random_device{}());
    for (auto _ = 0; _ < m; _++) {
        graph.add_edge(gen() % n, gen() % n);
    }

    for (auto start_v = 0; start_v < n; start_v++) {
        auto expected_distances = NAlgoLab::NBfs::seq_bfs(start_v, graph);

        ASSERT_EQ(NAlgoLab::NBfs::parallel_bfs(start_v, graph, 0, 10), expected_distances)
            << "Упала параллельная версия на стартовой вершине " << start_v;
    }
}

TEST(BfsTest, RandomDenseGraphTestParallelOnly) {
    // Здесь мы доверяем последовательному BFS
    auto n = 100;
    auto m = 3 * n * n;
    NAlgoLab::NUtils::TGraph graph(n);

    std::mt19937 gen(std::random_device{}());
    for (auto _ = 0; _ < m; _++) {
        graph.add_edge(gen() % n, gen() % n);
    }

    for (auto start_v = 0; start_v < n; start_v++) {
        auto expected_distances = NAlgoLab::NBfs::seq_bfs(start_v, graph);

        ASSERT_EQ(NAlgoLab::NBfs::parallel_bfs(start_v, graph, 0, 10), expected_distances)
            << "Упала параллельная версия на стартовой вершине " << start_v;
    }
}