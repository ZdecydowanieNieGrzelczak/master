#ifndef MASTER_GENERATION_H
#define MASTER_GENERATION_H

#include "Network.h"
#include "../games/Game.h"
#include "../utils/SharedDefinitions.h"





class Generation {
public:
    Generation(int generationCount, Game *game);
    Network iterateFor(int iterationCount);

    double testFor(int iterationCount, Network network);

private:

    void runThroughGeneration();
    std::vector<Network*> createNewGeneration(int bestIndex);

    std::vector<Network*> members;
    std::vector<float> memberScores;
    int generationCounter{0};
    float prevBest{0.0f};

    Game* game;

};

#endif //MASTER_GENERATION_H
