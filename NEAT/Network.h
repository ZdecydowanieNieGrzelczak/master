#ifndef MASTER_NETWORK_H
#define MASTER_NETWORK_H

#include <vector>
#include <unordered_set>
#include <map>
#include <cassert>
#include <iostream>
#include <memory>

#include "Connection.h"
#include "Neuron.h"
#include "../utils/SharedDefinitions.h"




class Network {
public:
    Network(int inputCount, int hiddenCount, int outputCount);

    Network(const Network &other);

    ~Network();

    int passThroughNetwork(const std::vector<float>& state);

    void mutate();

private:

    std::map<int, Neuron* > neuronMap;
    std::pair<std::vector<Connection *>, std::vector<Connection *>> connections;


};

#endif //MASTER_NETWORK_H
