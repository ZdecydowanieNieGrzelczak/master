
#include "Network.h"
#include "../utils/HelperMethods.h"



Network::Network(int inputCount, int outputCount) {
    int neuronCount = 1;
    for (auto layer : layers) {
        neurons[layer] = std::vector<Neuron>();
    }
    for (int x = 0; x < inputCount; ++x) {
        auto neuron = Neuron(neuronCount++, Layer::Input);

        neuronMap[neuron.getId()] = &neuron;
        neurons.at(Layer::Input).push_back(neuron);
    }

//    for (int x = 0; x < HIDDEN_LAYER_COUNT; ++x) {
//        auto neuron = new Neuron(neuronCount++, Layer::Hidden);
//        neuronMap[neuron->getId()] = neuron;
//        this->hidden.push_back(neuron);
//    }

    for (int x = 0; x < outputCount; ++x) {
        auto neuron = Neuron(neuronCount++, Layer::Output);
        neuronMap[neuron.getId()] = &neuron;
        neurons.at(Layer::Output).push_back(neuron);
    }
    int connectionCount = 0;
    connectLayers(neurons.at(Layer::Input), neurons.at(Layer::Output), connectionCount);

//    connectLayers(inputs, hidden);
//    connectLayers(hidden, outputs);

}



Network::Network(const Network &other) {
    neuronMap.clear();
    for(auto conn : other.connections) {
        auto inNeuron = getOrCreateNeuron(*other.neuronMap.at(conn.sourceId));
        auto outNeuron = getOrCreateNeuron(*other.neuronMap.at(conn.destinationId));

        this->connections.emplace_back(conn);
        inNeuron->addOutgoing(connections.back().getId());

    }

}


int Network::passThroughNetwork(const std::vector<float> &state) {
    assert(state.size() == neurons.at(Layer::Input).size());

    for (int x = 0; x < state.size(); ++x) {
        neurons.at(Layer::Input).at(x).receiveValue(state.at(x));
    }

    for (int x = 0; x < state.size(); ++x) {
        passNeuronValue(neurons.at(Layer::Input).at(x));
    }

    for (auto& neuron : neurons.at(Layer::Hidden)) {
        passNeuronValue(neuron);
    }

    assert(!neurons.at(Layer::Output).empty());

    double highestValue{-999999999999};
    int highestIndex{-1};
    for (int x{0}; x < neurons.at(Layer::Output).size(); ++x) {
        auto val = neurons.at(Layer::Output).at(x).getFinalValue();
        if (val > highestValue) {
            highestValue = val;
            highestIndex = x;
        }
    }
    assert(highestIndex >= 0);
    assert(highestIndex < neurons.at(Layer::Output).size());
    return highestIndex;
}

Network::~Network() = default;

void Network::passNeuronValue(Neuron& neuron) {
    const auto val = neuron.getFinalValue();
    for (auto conn : neuron.getOutputs()) {
        const auto destId = connectionMap.at(conn)->destinationId;
        neuronMap.at(destId)->receiveValue(val);
    }
}

void Network::mutate() {
    for (auto pair: neuronMap) {
        pair.second->mutate();
    }

    for (auto& conn : connections) {
        if (rand() % 100 <= 100 * CONNECTION_MUTATION_RATE) {
            conn.mutate();
        }
    }
}

Neuron* Network::getOrCreateNeuron(const Neuron& originalNeuron) {
    auto id = originalNeuron.getId();
    if (!neuronMap.contains(id)) {
        auto neuron = Neuron(originalNeuron);
        neuronMap[id] = &neuron;
        neurons.at(neuron.getLayer()).push_back(neuron);
        return &neuron;
    } else {
        return neuronMap[id];
    }
}

void Network::connectLayers(std::vector<Neuron>& in, const std::vector<Neuron>& out, int& connectionCount) {
    for (auto &srcNeuron : in) {
        for (auto &destNeuron : out) {
            auto newConn = new Connection(srcNeuron.getId(), destNeuron.getId(), ++connectionCount);
            connections.push_back(*newConn);
            connectionMap[connectionCount] = newConn;
            srcNeuron.addOutgoing(connectionCount);
        }
    }
}

