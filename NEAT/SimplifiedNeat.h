#ifndef MASTER_SIMPLIFIEDNEAT_H
#define MASTER_SIMPLIFIEDNEAT_H

#define SCORE_PENALTY_BASE 0.98
//#define GENERATION_MUTATION_PERC 0.01
#define GENERATION_MUTATION_PERC 0.25


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


#endif //MASTER_SIMPLIFIEDNEAT_H
