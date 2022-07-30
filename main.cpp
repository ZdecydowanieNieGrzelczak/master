#include <iostream>

#include "NEAT/Generation.h"
#include "games/tictactoe/TicTacToe.h"
#include "NEAT/SimplifiedNeat.h"
#include <omp.h>
#include <filesystem>
#include <unistd.h>

namespace fs = std::filesystem;

template <typename T>
void printVector(const std::vector<T>& vec) {
    for (T item : vec) {
        std::cout << item << std::endl;
    }
}

void printActions(const std::vector<std::pair<int, float>>& vec) {
    for (int i = 0; i < 3; ++i) {
        for (int x = 0; x < 3; ++x) {
            float val = vec[x + i * 3].second;
            std::cout << std::floor(val * 100.0) / 100.0 << "  | ";
        }
        std::cout << std::endl;
    }
}

int main() {
    omp_set_dynamic(0);     // Explicitly disable dynamic teams
    omp_set_num_threads(THREAD_NUM);

    auto game = new TicTacToe();
    srand (time(nullptr));
    auto generation = new Generation(POPULATION_COUNT, game);

    auto bestRes = generation->iterateFor(GENERATION_COUNT);
    fs::path save("newerData");
    fs::path dir(FILENAME_BASE);

    std::cout << "Saving scores" << std::endl;
    auto path = std::filesystem::current_path() / save / dir;

    if (fs::create_directories(path)) {
        path = path / dir;
    }

    generation->saveTheScores(path);
    std::cout << "Saving network" << std::endl;
    generation->saveTheNetwork(path.string());

//    omp_set_dynamic(0);     // Explicitly disable dynamic teams
//    omp_set_num_threads(1);
//
//    auto game = new TicTacToe();
//    srand (time(nullptr));
//
////    auto net = new SimplifiedNeat("../newData/relu-simp/relu-simp_network.csv");
//    auto net = new SimplifiedNeat("../newData/no-relu-simp/no-relu-simp_network.csv");
//
//
//    auto state = std::vector<float> {1, 0, 0,
//                                     0, 0, 0,
//                                     1, 0, 0,
//                                     ///////////
//                                     0, 1, 0,
//                                     0, 0, 0,
//                                     0, 0, 0};
//
//    auto res = net->passThroughNetwork(state);
//
//    std::cout << res << std::endl;
//
//
//    auto actions = net->passThroughNetworkWithActions(state);
//
//    printActions(actions);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
