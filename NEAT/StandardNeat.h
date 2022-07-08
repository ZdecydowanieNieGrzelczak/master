
#ifndef MASTER_STANDARDNEAT_H
#define MASTER_STANDARDNEAT_H


#include "Network.h"

class StandardNeat : public Network {
public:
    StandardNeat(int inputCount, int outputCount, int id);

    StandardNeat(const Network &other, int id);
    StandardNeat(const StandardNeat &other) = delete;
    StandardNeat(const Network &other) = delete;
    ~StandardNeat();

    std::pair<bool, int> mutate(int generation) override;
    void processBestNetwork(int generation) override;
    float getScoreModifier() override;

private:

    bool mutateStructure(int generation) override;
    bool createConnection(int generation) override;
    bool createNeuron(int generation) override;
};


#endif //MASTER_STANDARDNEAT_H
