#ifndef MASTER_GENERATION_H
#define MASTER_GENERATION_H

#include "Network.h"
#include "../games/Game.h"
#include "../utils/SharedDefinitions.h"
#include "Spiecie.h"


class Generation {
public:
    Generation(int generationCount, Game *game);
    Network * iterateFor(int iterationCount);

    double testFor(int iterationCount, Network &network);


    static void saveTheScore(const std::string &filename, std::vector<float> &scoreVec) ;

    void saveTheNetwork(const std::string &filename) const;


    void saveTheScores(const std::string& string);

private:

    void runThroughGeneration();

    std::vector<Network*> createNewGeneration(int bestIndex);

    std::vector<Network*> members;
    std::vector<float> memberScores;
    int generationCounter{0};

    Game* game;

    std::vector<float> generationScores;
    std::vector<float> generationRawScores;
    std::vector<int> spieciesCounter;
    std::vector<std::pair<int, int>> generationSizes;


    std::vector<Spiecie> spiecies;

    static void addToSpiecies(Network *net, std::vector<Spiecie> &newSpiecies);

    void saveTheSize(const std::string &filename) const;

    void saveTheSpiecies(const std::string &filename) const;
};

#endif //MASTER_GENERATION_H
