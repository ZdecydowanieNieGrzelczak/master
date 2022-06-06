#ifndef MASTER_SIMPLIFIEDNEAT_H
#define MASTER_SIMPLIFIEDNEAT_H


#include "Network.h"

class SimplifiedNeat : public Network {
public:
    SimplifiedNeat(int inputCount, int outputCount, int id);

    SimplifiedNeat(const SimplifiedNeat &other, int id);
    SimplifiedNeat(const SimplifiedNeat &other) = delete;
    ~SimplifiedNeat() override;

    std::pair<bool, int> mutate(int generation) override;


private:
    void pruneTheNetwork(int generation);
    bool pruneTheConnections(int generation);
    bool mutateStructure(int generation) override;
    bool createConnection(int generation) override;
    bool createNeuron(int generation) override;

    bool pruneNeurons(int generation);

    void deleteNeuron(Neuron *neuron);
};


#endif //MASTER_SIMPLIFIEDNEAT_H
