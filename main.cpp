#include <iostream>

#include "NEAT/Generation.h"
#include "games/tictactoe/TicTacToe.h"
#include <time.h>
// How many children we will have
#define POPULATION_COUNT 3

#define GENERATION_COUNT 1000
#define TEST_BATTLES_COUNT 10000

int main() {

    auto game = new TicTacToe();
    srand (time(nullptr));
    auto generation = new Generation(POPULATION_COUNT, game);
    auto bestRes = generation->iterateFor(GENERATION_COUNT);

    generation->testFor(TEST_BATTLES_COUNT, bestRes);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
