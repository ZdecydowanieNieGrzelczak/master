
#include "Network.h"
#include "../utils/HelperMethods.h"


#define HIDDEN_LAYER_COUNT 4

Network::Network(int inputCount, int outputCount) {
    int neuronCount = 0;
    for (int x = 0; x < inputCount; ++x) {
        auto neuron = new Neuron(neuronCount++, Layer::Input);
        neuronMap[neuron->getId()] = neuron;
        this->inputs.push_back(neuron);
    }

    for (int x = 0; x < HIDDEN_LAYER_COUNT; ++x) {
        auto neuron = new Neuron(neuronCount++, Layer::Hidden);
        neuronMap[neuron->getId()] = neuron;
        this->hidden.push_back(neuron);
    }

    for (int x = 0; x < outputCount; ++x) {
        auto neuron = new Neuron(neuronCount++, Layer::Output);
        neuronMap[neuron->getId()] = neuron;
        this->outputs.push_back(neuron);
    }

    connectLayers(inputs, hidden);
    connectLayers(outputs, hidden);

}



Network::Network(const Network &other) {
    std::unordered_set<int> createdNeurons;
    for(auto conn : other.connections) {
        auto inNeuron = getOrCreateNeuron(*conn->source, createdNeurons);
        auto outNeuron = getOrCreateNeuron(*conn->destination, createdNeurons);
        auto connection = new Connection(inNeuron, outNeuron, conn->getWeight());

//        outNeuron->addIncoming(conn);
        inNeuron->addOutgoing(conn);

        this->connections.push_back(connection);
    }

}


int Network::passThroughNetwork(const std::vector<float> &state) {
    assert(state.size() == inputs.size());
    std::cout << "running" << std::endl;

    for (int x = 0; x < state.size(); ++x) {
        inputs.at(x)->receiveValue(state.at(x));
    }
    std::cout << "running 2" << std::endl;


    for (int x = 0; x < state.size(); ++x) {
        inputs.at(x)->passValue();
    }

    std::cout << "running 3" << std::endl;


    for (auto neuron : hidden) {
        neuron->passValue();
    }

    assert(!outputs.empty());

    double highestValue = -999999999999;
    int highestIndex = -1;
    for (int x = 0; x < outputs.size(); ++x) {
        auto val = outputs.at(x)->getFinalValue();
        if (val > highestValue) {
            highestValue = val;
            highestIndex = x;
        }
    }
    assert(highestIndex >= 0);
    assert(highestIndex < outputs.size());
    return highestIndex;
}

Network::~Network() {
    for (auto neuron : inputs) {
        delete neuron;
    }
    for (auto neuron : hidden) {
        delete neuron;
    }
    for (auto neuron : outputs) {
        delete neuron;
    }
    for (auto conn : connections) {
        delete conn;
    }
}

void Network::mutate() {
    for (auto neuron : inputs) {
        if (rand() % 100 <= NEURON_MUTATION_RATE * 100) {
            neuron->mutate();
        }
    }

    for (auto neuron : hidden) {
        if (rand() % 100 <= 100 * NEURON_MUTATION_RATE) {
            neuron->mutate();
        }
    }

    for (auto neuron : outputs) {
        if (rand() % 100 <= 100 * NEURON_MUTATION_RATE) {
            neuron->mutate();
        }
    }

    for (auto conn : connections) {
        if (rand() % 100 <= 100 * CONNECTION_MUTATION_RATE) {
            conn->mutate();
        }
    }
}

Neuron* Network::getOrCreateNeuron(const Neuron& originalNeuron, std::unordered_set<int> &createdNeurons) {
    auto id = originalNeuron.getId();
    if (!createdNeurons.contains(id)) {
        createdNeurons.insert(id);
        auto neuron = new Neuron(originalNeuron);
        neuronMap[neuron->getId()] = neuron;
        switch (neuron->getLayer()) {
            case Layer::Hidden:
                hidden.push_back(neuron);
                break;
            case Layer::Input:
                inputs.push_back(neuron);
                break;
            case Layer::Output:
                outputs.push_back(neuron);
                break;
            default:
                std::cout << "This is fuckeeed" << std::endl;
        }
        return neuron;
    } else {
        return neuronMap[id];
    }
}

void Network::connectLayers(std::vector<Neuron *> in, const std::vector<Neuron *>& out) {
    for (auto & x : in) {
        for (auto & y : out) {
            auto connection = new Connection(x, y);
            connections.push_back(connection);
            x->addOutgoing(connection);
//            y->addIncoming(connection);
        }
    }
}

