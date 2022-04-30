
#include "Network.h"


Network::Network(int inputCount, int outputCount) {
    int neuronCount = 0;
    for (int x = 0; x < inputCount; ++x) {
        auto neuron = new Neuron(neuronCount++, Layer::Input);
        neuronMap[neuron->getId()] = neuron;
        this->inputs.push_back(neuron);
    }
    for (int x = 0; x < outputCount; ++x) {
        auto neuron = new Neuron(neuronCount++, Layer::Output);
        neuronMap[neuron->getId()] = neuron;
        this->outputs.push_back(neuron);
    }

    for (int x = 0; x < inputCount; ++x) {
        for (int y = 0; y < outputCount; ++y) {

            auto connection = new Connection(this->inputs[x], this->outputs[y]);
            this->connections.push_back(connection);
            this->inputs.at(x)->addOutgoing(connection);
            this->outputs[y]->addIncoming(connection);
        }
    }

}

Network::Network(const Network &other) {
    std::unordered_set<int> createdNeurons;
    for(auto conn : other.connections) {
        auto id = conn->source->getId();
        if (!createdNeurons.contains(id)) {
            createdNeurons.insert(id);
            auto originalNeuron = conn->source;
            auto neuron = new Neuron(*originalNeuron);
            neuronMap[neuron->getId()] = neuron;
            switch(neuron->getLayer()) {
                case Layer::Hidden:
                    hidden.push_back(neuron);
                    std::cout << "Hidden! " << std::endl;
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
            neuron->addOutgoing(conn);
        }
        id = conn->destination->getId();
        if (!createdNeurons.contains(id)) {
            createdNeurons.insert(id);
            auto originalNeuron = conn->destination;
            auto neuron = new Neuron(*originalNeuron);
            neuronMap[neuron->getId()] = neuron;
            switch(neuron->getLayer()) {
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
            neuron->addIncoming(conn);

        }
        auto connection = new Connection(neuronMap[conn->source->getId()], neuronMap[conn->destination->getId()], conn->getWeight());
        this->connections.push_back(connection);
    }

//    std::cout << "Copying network!" << std::endl;
//    std::cout << "set: " << createdNeurons.size() << std::endl;
//    std::cout << "Size of inputs: " << inputs.size() << std::endl;
//    std::cout << "Size of outputs: " << outputs.size() << std::endl;
//    std::cout << "Size of connections: " << connections.size() << std::endl;
//    std::cout << "______________________" << std::endl;
////
//    std::cout << "Other network!" << std::endl;
//    std::cout << "Size of inputs: " << other.inputs.size() << std::endl;
//    std::cout << "Size of outputs: " << other.outputs.size() << std::endl;
//    std::cout << "Size of connections: " << other.connections.size() << std::endl;
//    std::cout << "______________________" << std::endl;
}


int Network::passThroughNetwork(const std::vector<float> &state) {
    assert(state.size() == inputs.size());

    for (int x = 0; x < state.size(); ++x) {
        inputs.at(x)->receiveValue(state.at(x));
        inputs.at(x)->passValue();
    }

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
    std::cout << "mutating" << std::endl;
    for (auto neuron : inputs) {
        if (rand() % 100 / 100 > NEURON_MUTATION_RATE) {
            neuron->mutate();
        }
    }

    for (auto neuron : hidden) {
        if (rand() % 100 / 100 > NEURON_MUTATION_RATE) {
            neuron->mutate();
        }
    }

    for (auto neuron : outputs) {
        if (rand() % 100 / 100 > NEURON_MUTATION_RATE) {
            neuron->mutate();
        }
    }

    for (auto conn : connections) {
        if (rand() % 100 / 100 > CONNECTION_MUTATION_RATE) {
            conn->mutate();
        }
    }
}

