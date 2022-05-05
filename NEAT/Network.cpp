
#include "Network.h"

#include <memory>
#include "../utils/HelperMethods.h"



Network::Network(int inputCount, int hiddenCount, int outputCount) {
    int neuronCount = 0;
    for ( ; neuronCount < inputCount; ++neuronCount) {
        neuronMap[neuronCount] = new Neuron(neuronCount, Layer::Input);
    }
    for ( ; neuronCount < inputCount + hiddenCount; ++neuronCount) {
        neuronMap[neuronCount] = new Neuron(neuronCount, Layer::Hidden);
    }
    for ( ; neuronCount < inputCount + hiddenCount + outputCount; ++neuronCount) {
        neuronMap[neuronCount] = new Neuron(neuronCount, Layer::Output);
    }

    auto fromInputs = std::vector<Connection *>();
    auto fromHidden = std::vector<Connection *>();
    for (int x = 0; x < hiddenCount; ++x) {
        for (int y = 0; y < inputCount; ++y) {
            auto pConn = new Connection(y, x, 0);
            fromInputs.push_back(pConn);
        }
    }
    for (int x = inputCount; x < hiddenCount + inputCount; ++x) {
        for (int y = inputCount + hiddenCount; y < inputCount + hiddenCount + outputCount; ++y) {
            auto pConn = new Connection(y, x, 0);
            fromHidden.push_back(pConn);
        }
    }
    connections = std::make_pair(fromInputs, fromHidden);

}



Network::Network(const Network &other) {
    auto first = std::vector<Connection*>();
    auto second = std::vector<Connection*>();
    for(auto pConn : other.connections.first) {
        auto connection = new Connection(*pConn);
        first.push_back(connection);
    }
    for(auto pConn : other.connections.second) {
        auto connection = new Connection(*pConn);
        second.push_back(connection);
    }
    connections = std::make_pair(first, second);

    for (auto const& [id, pNeuron] : other.neuronMap) {
        neuronMap[id] = new Neuron(*pNeuron);
    }

}


int Network::passThroughNetwork(const std::vector<float> &state) {
    assert(state.size() == INPUT_LAYER_COUNT);

    for (int x = 0; x < state.size(); ++x) {
        neuronMap[x]->receiveValue(state.at(x));
    }

    for (auto pConn : connections.first) {
        auto val = neuronMap[pConn->sourceId]->getFinalValue();
        neuronMap[pConn->destinationId]->receiveValue(val);
    }

    for (auto pConn : connections.second) {
        auto val = neuronMap[pConn->sourceId]->getFinalValue();
        neuronMap[pConn->destinationId]->receiveValue(val);
    }

    double highestValue = -999999999999;
    int highestIndex = -1;
    for (int x = neuronMap.size() - OUTPUT_LAYER_COUNT; x < neuronMap.size(); ++x) {
        auto val = neuronMap[x]->getFinalValue();
        if (val > highestValue) {
            highestValue = val;
            highestIndex = x;
        }
    }
    highestIndex += OUTPUT_LAYER_COUNT - neuronMap.size();
    assert(highestIndex >= 0);
    assert(highestIndex < OUTPUT_LAYER_COUNT);
    return highestIndex;
}

Network::~Network() {
    for (auto pConn : connections.first) {
        delete pConn;
    }
    for (auto pConn : connections.second) {
        delete pConn;
    }
    for (auto &[_, pNeur] : neuronMap) {
        delete pNeur;
    }
}

void Network::mutate() {
    for (auto &[id, pNeuron] : neuronMap) {
        if (HelperMethods::getRandomChance() <= NEURON_MUTATION_PERCENTAGE_CHANCE) {
            pNeuron->mutate();
        }
    }
    for (auto pConn : connections.first) {
        if (HelperMethods::getRandomChance() <= CONNECTION_MUTATION_PERCENTAGE_CHANCE) {
            pConn->mutate();
        }
    }
    for (auto pConn : connections.second) {
        if (HelperMethods::getRandomChance() <= CONNECTION_MUTATION_PERCENTAGE_CHANCE) {
            pConn->mutate();
        }
    }
}


