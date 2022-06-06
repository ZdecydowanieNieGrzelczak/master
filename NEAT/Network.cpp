
#include <ranges>
#include "Network.h"



//Network::Network(int inputCount, int outputCount, StructureMutator* ledger): ledger{ledger} {
Network::Network(int inputCount, int outputCount, int id): parentId{id}, id{id} {
    int neuronCount = 1;
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

    connectLayers(inputs, outputs, 0);

}



Network::Network(const Network &other, int id): id{id}, parentId{other.id} {
    neuronMap.clear();
    connections.clear();
    for(auto &[ID, conn] : other.connections) {
        auto inNeuron = getOrCreateNeuron(*conn->source);
        auto outNeuron = getOrCreateNeuron(*conn->destination);
        auto connection = new Connection(inNeuron, outNeuron, ID,
                                         conn->getWeight(), conn->isEnabled(), conn->isOriginal());

        inNeuron->addOutgoing(connection);

        this->connections[ID] = connection;
        for (auto id : other.connInnovations) {
            connInnovations.insert(id);
        }
    }

}


int Network::passThroughNetwork(const std::vector<float> &state) {
    assert(state.size() == inputs.size());

    for (int x = 0; x < state.size(); ++x) {
        inputs.at(x)->receiveValue(state.at(x));
    }


    for (int x = 0; x < state.size(); ++x) {
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


void Network::mutateWeights() {
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

    for (auto & [ID, conn] : connections) {
        if (rand() % 100 <= 100 * CONNECTION_MUTATION_RATE) {
            conn->mutate();
        }
    }
}

void Network::toggleConnection() {

    std::pair<int, Connection*> pair;
    std::mt19937 gen( std::random_device{}() );
    std::sample(connections.begin(), connections.end(), &pair, 1, gen );
    pair.second->toggle();

}



Neuron* Network::getOrCreateNeuron(const Neuron& originalNeuron) {
    auto originalId = originalNeuron.getId();
    if (!neuronMap.contains(originalId)) {
        auto neuron = new Neuron(originalNeuron);
        neuronMap[originalId] = neuron;
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
        return neuronMap[originalId];
    }
}

int Network::connectLayers(std::vector<Neuron *> &in, const std::vector<Neuron *> &out, int current) {
    for (auto & srcNeuron : in) {
        for (auto & destNeuron : out) {
            auto connection = new Connection(srcNeuron, destNeuron, current++, true);
            connections[current] = (connection);
            srcNeuron->addOutgoing(connection);
        }
    }
    return current;
}

std::string Network::getSaveData() {
    std::string data = "Network\n";
    data += "Inputs\n";
    for (auto input : inputs) {
        data += std::to_string(input->getBias()) + ";" +  std::to_string(input->getId()) + "\n";
    }
    data += "Hidden\n";
    for (auto hidd : hidden) {
        data += std::to_string(hidd->getBias()) + ";" + std::to_string(hidd->getId()) + "\n";
    }
    data += "Output\n";
    for (auto output : outputs) {
        data += std::to_string(output->getBias()) + ";" + std::to_string(output->getId()) + "\n";
    }
    data += "Connection\n";
    for (auto & [connID, conn] : connections) {
        data += std::to_string(conn->getWeight()) + ";" + std::to_string(conn->source->getId()) + ";" +
                std::to_string(conn->destination->getId()) + ";" + std::to_string(connID) + ";" +
                std::to_string(conn->isEnabled() ? 1 : 0) + "\n";
    }
    return data;
}


Neuron* Network::getRandomNeuron(Layer layer) {
    switch (layer) {
        case Layer::Input:
            return inputs[HelperMethods::getRandomInt(0, inputs.size())];
        case Layer::Hidden:
            return hidden[HelperMethods::getRandomInt(0, hidden.size())];
        case Layer::Output:
            return outputs[HelperMethods::getRandomInt(0, outputs.size())];
        default:
            return nullptr;
    }
}



double Network::getSimilarity(const Network &network) {
    if (parentId == network.parentId) {
        return 100.0;
    }

    if (hidden.size() != network.hidden.size()) {
        return 0.0;
    }

    if (std::abs(int(connInnovations.size() - network.connInnovations.size())) > 0) {
        return 0.0;
    }

    for (auto connIn : connInnovations ) {
        if (!network.connInnovations.contains(connIn)) {
            return false;
        }
    }

    for (auto connIn : network.connInnovations ) {
        if (!connInnovations.contains(connIn)) {
            return false;
        }
    }

    double connectionDiff{0.0};


    for (auto [ID, connection] : connections) {
        connectionDiff += abs(connection->getWeight() * connection->isEnabled() - network.connections.at(ID)->getWeight() * connection->isEnabled());
    }


    return std::max(10 - connectionDiff, 0.0);
}
