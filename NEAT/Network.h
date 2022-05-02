#ifndef MASTER_NETWORK_H
#define MASTER_NETWORK_H

#include <vector>
#include <unordered_set>
#include <map>
#include <cassert>
#include <iostream>

#include "Connection.h"
#include "Neuron.h"
#include "../utils/SharedDefinitions.h"




class Network {
public:
    Network(int inputCount, int outputCount);

    Network(const Network &other);

    ~Network();

    int passThroughNetwork(const std::vector<float>& state);

    void mutate();

private:
    Neuron *getOrCreateNeuron(const Neuron &originalNeuron);
    void connectLayers(std::vector<Neuron *>& in, const std::vector<Neuron *>& out);


    std::vector<Neuron*> inputs;
    std::vector<Neuron*> hidden;
    std::vector<Neuron*> outputs;

    std::map<int, Neuron*> neuronMap;

    std::vector<Connection*> connections;

};

#endif //MASTER_NETWORK_H
