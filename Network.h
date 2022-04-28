#ifndef MASTER_NETWORK_H
#define MASTER_NETWORK_H

#include <vector>
#include "Neuron.h"
#include "Connection.h"

class Network {
public:
    Network(int inputCount, int outputCount);


private:
    std::vector<Neuron*> inputs;
    std::vector<Neuron*> hidden;
    std::vector<Neuron*> outputs;

    std::vector<Connection*> connections;
};

#endif //MASTER_NETWORK_H
