#include <iostream>
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
    return checkConnectionInnovation(std::pair(neuronFromId, neronToId));
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

int StructureMutator::getOrCreateConnInnovation(int key) {
    if (neuronInnovations.contains(key)){
        return neuronInnovations.at(key);
    }
    auto innovationNumber = neuronInnovationCounter++;
    neuronInnovations[key] = innovationNumber;
    return innovationNumber;
}

int StructureMutator::getNewNeuronId() {
    return ++neuronInnovationCounter;
}

void StructureMutator::print() {
    for (auto & [key, id] : connectionsInnovations) {
        std::cout << "Connection from: " << key.first << " to " << key.second << " has id: " << id << std::endl;
    }
}