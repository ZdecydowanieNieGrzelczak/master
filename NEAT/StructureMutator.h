#ifndef MASTER_STRUCTUREMUTATOR_H
#define MASTER_STRUCTUREMUTATOR_H

#include <map>
#include "../utils/SharedDefinitions.h"



class StructureMutator {
public:
    static StructureMutator& getInstance() {
        static StructureMutator instance;
        return instance;
    }

    int checkConnectionInnovation(int neuronFromId, int neronToId) const;
    int checkConnectionInnovation(const std::pair<int, int> &key) const;

    int checkNeuronInnovation(int neuronFromId, int neronToId) const;
    int checkNeuronInnovation(const std::pair<int, int> &key) const;

    std::map<std::pair<int, int>, int> neuronInnovations;
    std::map<std::pair<int, int>, int> connectionsInnovations;

    int connInnovationCounter{0};
    int neuronInnovationCounter{0};

    int getOrCreateConnInnovation(std::pair<int, int> key);

    int getNewNeuronId();
};

extern StructureMutator* ledger;


#endif //MASTER_STRUCTUREMUTATOR_H
