#ifndef MASTER_NETWORK_H
#define MASTER_NETWORK_H

#include <vector>
#include <unordered_set>
#include <map>


#include "Connection.h"
#include "Neuron.h"

#define CONNECTION_MUTATION_RATE 0.2
#define NEURON_MUTATION_RATE 0.2


static int neuronCount{0};

class Network {
public:
    Network(int inputCount, int outputCount);

    Network(const Network &other);

    ~Network();

    int passThroughNetwork(const std::vector<float>& state);
private:
    std::vector<Neuron*> inputs;
    std::vector<Neuron*> hidden;
    std::vector<Neuron*> outputs;

    std::map<int, Neuron*> neuronMap;

    std::vector<Connection*> connections;

    void mutate();

};

#endif //MASTER_NETWORK_H
