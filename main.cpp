#include <iostream>

#include "NEAT/Generation.h"
#include "games/tictactoe/TicTacToe.h"
#include <omp.h>


int main() {
    omp_set_dynamic(0);     // Explicitly disable dynamic teams
    omp_set_num_threads(THREAD_NUM);

    auto game = new TicTacToe();
    srand (time(nullptr));
    auto generation = new Generation(POPULATION_COUNT, game);

    auto bestRes = generation->iterateFor(GENERATION_COUNT);



    std::cout << "Saving scores" << std::endl;
    generation->saveTheScores(FILENAME_BASE);
    std::cout << "Saving network" << std::endl;
    generation->saveTheNetwork(FILENAME_BASE);

    ledger->print();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
