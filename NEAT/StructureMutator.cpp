#include "StructureMutator.h"


int StructureMutator::checkConnectionInnovation(int neuronFromId, int neronToId) const {
    return checkConnectionInnovation(std::pair(neronToId, neronToId));
}

int StructureMutator::checkConnectionInnovation(const std::pair<int, int> &key) const {
    if (connectionsInnovations.contains(key)){
        return connectionsInnovations.at(key);
    }
    return NOT_FOUND_CODE;
}

int StructureMutator::checkNeuronInnovation(int neuronFromId, int neronToId) const {
    return checkConnectionInnovation(std::pair(neronToId, neronToId));
}

int StructureMutator::checkNeuronInnovation(const std::pair<int, int> &key) const {
    if (connectionsInnovations.contains(key)){
        return connectionsInnovations.at(key);
    }
    return NOT_FOUND_CODE;
}

int StructureMutator::getOrCreateConnInnovation(std::pair<int, int> key) {
    if (connectionsInnovations.contains(key)){
        return connectionsInnovations.at(key);
    }
    auto innovationNumber = connInnovationCounter++;
    connectionsInnovations[key] = innovationNumber;
    return innovationNumber;
}

