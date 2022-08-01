#ifndef MASTER_SIMPLIFIEDNEAT_H
#define MASTER_SIMPLIFIEDNEAT_H

#define SCORE_PENALTY_BASE 0.995
//#define GENERATION_MUTATION_PERC 0.01
#define GENERATION_MUTATION_PERC 0.15
#define NO_EXPANSION_INTERVAL 100
#define EXPANSION_INTERVAL 50


#include "Network.h"

class SimplifiedNeat : public Network {
public:
    SimplifiedNeat(int inputCount, int outputCount, int id);

    SimplifiedNeat(const Network &other, int id);
    SimplifiedNeat(const Network &left, const Network &right, int id);
    SimplifiedNeat(const std::string& filename);
    SimplifiedNeat(const SimplifiedNeat &other) = delete;
    SimplifiedNeat(const Network &other) = delete;
    ~SimplifiedNeat();

    std::pair<bool, int> mutate(int generation) override;
    void processBestNetwork(int generation) override;
    float getScoreModifier() override;

private:
    void pruneTheNetwork(int generation);
    bool pruneTheConnections(int generation);
    bool mutateStructure(int generation) override;
    bool createConnection(int generation) override;
    bool createNeuron(int generation) override;

    bool pruneNeurons(int generation);

};

int changeAt{(int)std::floor(GENERATION_MUTATION_PERC * GENERATION_COUNT)};
bool isStructureMutationPermitted{false};

#endif //MASTER_SIMPLIFIEDNEAT_H
