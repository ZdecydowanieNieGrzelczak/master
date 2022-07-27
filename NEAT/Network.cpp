
#include "Network.h"
#include <ranges>
#include <algorithm>

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

    connectLayers(inputs, outputs);

    originalConnectionsCount = connections.size();

}



Network::Network(const Network &other, int id): id{id}, parentId{other.id}, originalConnectionsCount{other.originalConnectionsCount} {
//    neuronMap.clear();
//    connections.clear();
    for(auto &[ID, conn] : other.connections) {
        auto inNeuron = getOrCreateNeuron(*conn->source);
        auto outNeuron = getOrCreateNeuron(*conn->destination);
        auto connection = new Connection(inNeuron, outNeuron, *conn);

        inNeuron->addOutgoing(connection);

        this->connections[ID] = connection;

    }
    for (auto cid : other.connInnovations) {
        connInnovations.insert(cid);
    }

}


//Network::Network(const Network& left, const Network &right, int id): Network(left, id) {
Network::Network(const Network& left, const Network &right, int id): id{id}, originalConnectionsCount{left.originalConnectionsCount}, parentId{left.id} {
//    neuronMap.clear();
//    connections.clear();
//    connInnovations.clear();

    for(int i = 0; i < left.originalConnectionsCount; ++i) {
        auto conn = left.connections.at(i);
        auto inNeuron = getOrCreateNeuron(*conn->source);
        auto outNeuron = getOrCreateNeuron(*conn->destination);
        auto connRight = right.connections.at(i);
        inNeuron->mergeWithNeuron(connRight->source);
        outNeuron->mergeWithNeuron(connRight->destination);

        auto connection = new Connection(inNeuron, outNeuron, *conn);
        connection->mergeWithConnection(connRight);
        inNeuron->addOutgoing(connection);

        this->connections[i] = connection;

    }
    for (auto cid : left.connInnovations) {
        auto conn = left.connections.at(cid);
        auto inNeuron = getOrCreateNeuron(*conn->source);
        auto outNeuron = getOrCreateNeuron(*conn->destination);
        auto connection = new Connection(inNeuron, outNeuron, *conn);


        if(right.connInnovations.contains(cid)) {
            auto connRight = right.connections.at(cid);
            inNeuron->mergeWithNeuron(connRight->source);
            outNeuron->mergeWithNeuron(connRight->destination);
            connection->mergeWithConnection(connRight);

        }
        this->connections[cid] = connection;
        connInnovations.insert(cid);
    }

    for (auto cid : right.connInnovations) {
        if (!connInnovations.contains(cid)) {
            auto conn = right.connections.at(cid);
            auto inNeuron = getOrCreateNeuron(*conn->source);
            auto outNeuron = getOrCreateNeuron(*conn->destination);
            auto connection = new Connection(inNeuron, outNeuron, *conn);
            this->connections[cid] = connection;
            connInnovations.insert(cid);
        }
    }

}



int Network::passThroughNetwork(const std::vector<float> &state) {
//    assert(state.size() == inputs.size());

    for (int x = 0; x < state.size(); ++x) {
        inputs.at(x)->receiveValue(state.at(x));
    }


    for (int x = 0; x < state.size(); ++x) {
        inputs.at(x)->passValue();
    }

    for (auto neuron : hidden) {
        neuron->passValue();
    }

//    assert(!outputs.empty());

    double highestValue = -999999999999;
    int highestIndex = -1;
    for (int x = 0; x < outputs.size(); ++x) {
        auto val = outputs.at(x)->getFinalValue();
        if (val > highestValue) {
            highestValue = val;
            highestIndex = x;
        }
    }
//    assert(highestIndex >= 0);
//    assert(highestIndex < outputs.size());
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

void Network::toggleConnection(int generation) {

    std::pair<int, Connection*> pair;
    std::mt19937 gen( std::random_device{}() );
    std::sample(connections.begin(), connections.end(), &pair, 1, gen );
    pair.second->toggle(generation);

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

void Network::connectLayers(std::vector<Neuron *> &in, const std::vector<Neuron *> &out) {
    for (auto & srcNeuron : in) {
        for (auto & destNeuron : out) {
            auto connId = ledger->getOrCreateConnInnovation({srcNeuron->getId(), destNeuron->getId()});
            auto connection = new Connection(srcNeuron, destNeuron, connId, true);
            connections[connId] = (connection);
            srcNeuron->addOutgoing(connection);
        }
    }
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


//    for (auto [ID, connection] : connections) {
//        connectionDiff += abs(connection->getWeight() * connection->isEnabled() - network.connections.at(ID)->getWeight() * connection->isEnabled());
//    }


    return std::max(15 - connectionDiff, 0.0);
}

void Network::deleteConnection(int cid, bool fromNeuron) {
    connInnovations.erase(cid);
    auto conn = connections[cid];
    conn->source->removeFromOutgoing(conn);
    delete conn;
    connections.erase(cid);

}


void Network::deleteNeuron(int neuronID) {
    std::vector<int> toRemove;
    for (auto & [connId, conn] : connections) {
        if (conn->source->getId() == neuronID || conn->destination->getId() == neuronID) {
            toRemove.push_back(connId);

        }
    }
    for (auto connId : toRemove) {
        deleteConnection(connId, true);
    }
    auto neuron = neuronMap[neuronID];
    neuronMap.erase(neuronID);
    hidden.erase(std::remove(hidden.begin(), hidden.end(), neuron), hidden.end());
//    hidden.erase(std::remove_if(hidden.begin(), hidden.end(), [neuronID](Neuron *n){return n->getId() == neuronID; }), hidden.end());
    delete neuron;
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
    for (auto &[ID, conn] : connections) {
        delete conn;
    }
}

int Network::getAdditionalSize() {
    return connections.size() - originalConnectionsCount + hidden.size();
}


