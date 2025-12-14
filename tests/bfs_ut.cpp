#include <gtest/gtest.h>

#include "../src/bfs.h"
#include "../src/utils.h"

TEST(BfsTest, CubeGraphResultCorrectness) {
    int n = 100;
    int N = n * n * n;
    auto graph = NAlgoLab::NUtils::generate_cube_graph(n);

    auto seq_result = NAlgoLab::NBfs::seq_bfs(0, graph);
    auto par_result = NAlgoLab::NBfs::parallel_bfs(0, graph, 0, n / 10);

    for (int v = 0; v < N; v++) {
        auto [x, y, z] = NAlgoLab::NUtils::get_coord(v, n);
        ASSERT_EQ(seq_result[v], x + y + z);
        ASSERT_EQ(par_result[v], x + y + z);
    }
}
