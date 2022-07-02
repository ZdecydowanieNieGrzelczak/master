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
    Network(int inputCount, int outputCount, int id);

    Network(const Network &other, int id);
    Network(const Network &other) = delete;

    ~Network();

    int passThroughNetwork(const std::vector<float>& state);

    virtual std::pair<bool, int> mutate(int generation) = 0;
    virtual void processBestNetwork(int generation) = 0;

    std::string getSaveData();

    size_t getHiddenSize() {return hidden.size(); };
    size_t getConnectionsSize() {return connections.size(); };

    int spiecieID{0};

    int originalConnectionsCount{0};

    double getSimilarity(const Network &network);


protected:
    void mutateWeights();
    void toggleConnection(int generation);

    void deleteConnection(int cid, bool fromNeuron);

    virtual bool mutateStructure(int generation) = 0;
    virtual bool createConnection(int generation) = 0;
    virtual bool createNeuron(int generation) = 0;

    std::vector<Neuron*> inputs;
    std::vector<Neuron*> hidden;
    std::vector<Neuron*> outputs;

    std::map<int, Neuron*> neuronMap;

    std::map<int, Connection*> connections;

    std::unordered_set<int> connInnovations;
    std::unordered_set<int> neuronInnovations;

    Neuron *getRandomNeuron(Layer layer);

    void deleteNeuron(int neuronID);


private:
    Neuron *getOrCreateNeuron(const Neuron &originalNeuron);
    int connectLayers(std::vector<Neuron *> &in, const std::vector<Neuron *> &out, int current);

    int parentId;
    int id;


};

#endif //MASTER_NETWORK_H
