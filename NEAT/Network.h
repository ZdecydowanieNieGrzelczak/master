#ifndef MASTER_NETWORK_H
#define MASTER_NETWORK_H

#include <vector>
#include <unordered_set>
#include <map>
#include <cassert>
#include <iostream>
#include <stdlib.h>

#include "Connection.h"
#include "Neuron.h"
#include "../utils/SharedDefinitions.h"
#include "StructureMutator.h"



class Network {
public:
//    Network(int inputCount, int outputCount, StructureMutator* ledger);
    Network(int inputCount, int outputCount, int id);

    Network(const Network &other, int id);

    ~Network();

    int passThroughNetwork(const std::vector<float>& state);

    std::pair<bool, int> mutate();

    std::string getSaveData();

    size_t getHiddenSize() {return hidden.size(); };
    size_t getConnectionsSize() {return connections.size(); };

    double getSimilarity(const Network &network);

private:
    Neuron *getOrCreateNeuron(const Neuron &originalNeuron);
    int connectLayers(std::vector<Neuron *>& in, const std::vector<Neuron *>& out, bool original, int current);

//    StructureMutator* ledger;

    std::vector<Neuron*> inputs;
    std::vector<Neuron*> hidden;
    std::vector<Neuron*> outputs;

    std::map<int, Neuron*> neuronMap;

    std::vector<Connection*> connections;

    std::unordered_set<int> connInnovations;
    std::unordered_set<int> neuronInnovations;

    bool createConnection();
    bool createNeuron();


    Neuron *getRandomNeuron(Layer layer);

    void mutateWeights();
    void toggleConnection();
    bool mutateStructure();

    int parentId;
    int id;
};

#endif //MASTER_NETWORK_H
