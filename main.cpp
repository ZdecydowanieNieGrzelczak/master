#include <iostream>

#include "NEAT/Generation.h"
#include "games/tictactoe/TicTacToe.h"



int main() {
    auto game = new TicTacToe();
    srand (time(nullptr));
    auto generation = new Generation(POPULATION_COUNT, game);

    auto bestRes = generation->iterateFor(GENERATION_COUNT);


    std::cout << "Testing" << std::endl;

    generation->testFor(TEST_BATTLES_COUNT, *bestRes);

    std::cout << "Saving scores" << std::endl;
    generation->saveTheScores(FILENAME_BASE);
    std::cout << "Saving network" << std::endl;
    generation->saveTheNetwork(FILENAME_BASE);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
