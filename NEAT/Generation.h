#ifndef MASTER_GENERATION_H
#define MASTER_GENERATION_H

#include <filesystem>
#include "Network.h"
#include "../games/Game.h"
#include "../utils/SharedDefinitions.h"
#include "Spiecie.h"


class Generation {
public:
    Generation(int generationCount, Game *game);
    Network * iterateFor(int iterationCount);

    double testFor(int iterationCount, Network &network);


    static void saveTheScore(const std::string& filename, std::vector<float> &scoreVec) ;

    void saveTheNetwork(const std::string& filename) const;


    void saveTheScores(const std::filesystem::__cxx11::path& string);
    double testForWithoutConstrains(int iterationCount, Network &network);

private:

    void runThroughGeneration();

    std::vector<Network*> createNewGeneration(int bestIndex);

    std::vector<Network*> members;
    std::vector<float> memberScores;
    int generationCounter{0};

    Game* game;

    std::vector<float> generationScores;
    std::vector<float> generationRawScores;
    std::vector<float> generationScoresAvg;
    std::vector<float> generationRawScoresAvg;
    std::vector<int> spieciesCounter;
    std::vector<std::pair<int, int>> generationSizes;
    std::vector<float> generationSizesAvg;


    std::vector<Spiecie> spiecies;

    static void addToSpiecies(Network *net, std::vector<Spiecie> &newSpiecies);

    void saveTheSize(const std::string& filename) const;

    void saveTheSpiecies(const std::string& filename) const;

    int getTournamentIndex();

    int changeAt{(int)std::floor(GENERATION_MUTATION_PERC * GENERATION_COUNT)};
    bool isStructureMutationPermitted{false};

};

#endif //MASTER_GENERATION_H
