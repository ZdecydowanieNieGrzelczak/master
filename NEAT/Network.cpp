
#include <ranges>
#include "Network.h"
#include "../utils/HelperMethods.h"



Network::Network(int inputCount, int outputCount, StructureMutator* ledger): ledger{ledger} {
    int neuronCount = 1;
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

    int current = connectLayers(inputs, hidden, true, 0);
    connectLayers(hidden, outputs, true, current);

}



Network::Network(const Network &other): ledger{other.ledger} {
    neuronMap.clear();
    for(auto conn : other.connections) {
        auto inNeuron = getOrCreateNeuron(*conn->source);
        auto outNeuron = getOrCreateNeuron(*conn->destination);
        auto connection = new Connection(*conn);

//        outNeuron->addIncoming(conn);
        inNeuron->addOutgoing(connection);

        this->connections.push_back(connection);
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

std::pair<bool, int> Network::mutate() {
    int roll = HelperMethods::getRandomChance();
    if ( roll < WEIGHTS_MUTATION_RATE ) {
        mutateWeights();
    } else if (roll < CONN_TOGGLING_RATE + WEIGHTS_MUTATION_RATE ) {
        toggleConnection();
    }
    else {
//    if (HelperMethods::getRandomChance() > STRUCTURE_MUTATION_RATE ) {
        return {true,  mutateStructure()};
    }
    return {false, 0};
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

    for (auto conn : connections) {
        if (rand() % 100 <= 100 * CONNECTION_MUTATION_RATE) {
            conn->mutate();
        }
    }
}

void Network::toggleConnection() {
    connections[HelperMethods::getRandomInt(0, connections.size())]->toggle();
}

bool Network::mutateStructure() {
    if (HelperMethods::getRandomChance() > NEURON_CREATION_RATE ) {
        return createNeuron();
    } else {
        return createConnection();
    }
}

Neuron* Network::getOrCreateNeuron(const Neuron& originalNeuron) {
    auto id = originalNeuron.getId();
    if (!neuronMap.contains(id)) {
        auto neuron = new Neuron(originalNeuron);
        neuronMap[id] = neuron;
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

int Network::connectLayers(std::vector<Neuron *>& in, const std::vector<Neuron *>& out, bool original, int current) {
    for (auto & srcNeuron : in) {
        for (auto & destNeuron : out) {
            auto connection = new Connection(srcNeuron, destNeuron, current++, original);
            connections.push_back(connection);
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
    for (auto conn : connections) {
        data += std::to_string(conn->getWeight()) + ";" + std::to_string(conn->source->getId()) + ";" +
                std::to_string(conn->destination->getId()) + "\n";
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
    }
}

bool Network::createConnection() {
    if (hidden.empty()) {
        return false;
    }
    bool fromInput = HelperMethods::getCoinFlip();

    Neuron* first = getRandomNeuron(fromInput ? Layer::Input : Layer::Hidden);
    Neuron* second = getRandomNeuron(fromInput ? Layer::Hidden : Layer::Output);

    auto key = std::make_pair(first->getId(), second->getId());
    int innovation = ledger->getOrCreateConnInnovation(key);
    while (connInnovations.contains(innovation)) {
        first = getRandomNeuron(fromInput ? Layer::Input : Layer::Hidden);
        second = getRandomNeuron(fromInput ? Layer::Hidden : Layer::Output);
        key = std::make_pair(first->getId(), second->getId());
        innovation = ledger->getOrCreateConnInnovation(key);
    }

    auto* conn = new Connection(first, second, innovation, 1.0, false, true);

    connInnovations.insert(innovation);
    connections.push_back(conn);
    return true;
}

bool Network::createNeuron() {
    std::vector<Connection*> possibilities;
    std::copy_if(connections.begin(), connections.end(), std::back_inserter(possibilities),
                  [](Connection* conn){ return conn->isOriginal() && conn->isEnabled(); });
    if (possibilities.empty()) {
        return false;
    }

    auto randomConn = possibilities[HelperMethods::getRandomInt(0, possibilities.size())];
    randomConn->disable();
    int newNeuronId = ledger->getNewNeuronId();
    auto newNeuron = new Neuron(newNeuronId, Layer::Hidden);
    auto connInInnovation = ledger->getOrCreateConnInnovation({randomConn->source->getId(), newNeuronId});
    auto newConnIn = new Connection(randomConn->source, newNeuron, connInInnovation, false);
    auto connOutInnovation = ledger->getOrCreateConnInnovation({newNeuronId, randomConn->destination->getId()});
    auto newConnOut = new Connection(randomConn->source, newNeuron, connInInnovation, false);

    randomConn->source->addOutgoing(newConnIn);
    newNeuron->addOutgoing(newConnOut);

    connections.push_back(newConnIn);
    connections.push_back(newConnOut);

    hidden.push_back(newNeuron);

    return true;
}
