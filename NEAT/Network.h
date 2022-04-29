#ifndef MASTER_NETWORK_H
#define MASTER_NETWORK_H

#include <vector>

#include "Connection.h"
#include "Neuron.h"

class Network {
public:
    Network(int inputCount, int outputCount);

    int passThroughNetwork(const std::vector<float>& state);
private:
    std::vector<Neuron*> inputs;
    std::vector<Neuron*> hidden;
    std::vector<Neuron*> outputs;

    std::vector<Connection*> connections;

};

#endif //MASTER_NETWORK_H
