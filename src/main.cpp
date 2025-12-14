#include <iostream>

#include "bfs.h"
#include "quicksort.h"

int main(int argc, char* argv[]) {
    assert(argc > 1);

    std::map<std::string, std::string> args;
    for(int i = 1; i < argc; i += 2) {
        std::string key = argv[i];
        std::string value = argv[i + 1];
        assert(key[0] == '-' && key[1] == '-');
        assert(args.find(key) == args.end());
        args[key] = value;
    }

    if (args["--mode"] == "quicksort") {
        int n = 1e8;
        if (auto it = args.find("--n"); it != args.end()) {
            n = std::stoi(it->second);
        }
        auto attempts = 5;
        if (auto it = args.find("--attempts"); it != args.end()) {
            attempts = std::stoi(it->second);
        }
        auto batch_size = 1 << 18;
        if (auto it = args.find("--batch-size"); it != args.end()) {
            batch_size = std::stoi(it->second);
        }
        NAlgoLab::NQuickSort::quicksort_benchmark(n, attempts, batch_size);
    } else if (args["--mode"] == "bfs") {
        int n = 300;
        if (auto it = args.find("--n"); it != args.end()) {
            n = std::stoi(it->second);
        }
        auto attempts = 5;
        if (auto it = args.find("--attempts"); it != args.end()) {
            attempts = std::stoi(it->second);
        }
        auto front_batch_treshold = 2000;
        if (auto it = args.find("--front-batch-treshold"); it != args.end()) {
            front_batch_treshold = std::stoi(it->second);
        }
        auto block_sz = 200;
        if (auto it = args.find("--block-size"); it != args.end()) {
            block_sz = std::stoi(it->second);
        }
        NAlgoLab::NBfs::bfs_cube_graph_benchmark(n, attempts, front_batch_treshold, block_sz);
    }
    return 0;
}
