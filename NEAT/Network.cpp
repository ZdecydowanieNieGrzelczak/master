
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

//    for (int x = 0; x < HIDDEN_LAYER_COUNT; ++x) {
//        auto neuron = new Neuron(neuronCount++, Layer::Hidden);
//        neuronMap[neuron->getId()] = neuron;
//        this->hidden.push_back(neuron);
//    }

    for (int x = 0; x < outputCount; ++x) {
        auto neuron = new Neuron(neuronCount++, Layer::Output);
        neuronMap[neuron->getId()] = neuron;
        this->outputs.push_back(neuron);
    }

//    int current = connectLayers(inputs, hidden, true, 0);
//    connectLayers(hidden, outputs, true, current);
    connectLayers(inputs, outputs, true, 0);

}



Network::Network(const Network &other, int id): id{id}, parentId{other.id} {
//Network::Network(const Network &other): ledger{other.ledger} {
    neuronMap.clear();
    connections.clear();
    for(auto  [ID, conn] : other.connections) {
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

bool Network::mutateStructure() {
    if (HelperMethods::getRandomChance() > NEURON_CREATION_RATE ) {
        return createNeuron();
    } else {
        return createConnection();
    }
    return false;
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
        default:
            return nullptr;
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
    int tries = 0;
    while (connInnovations.contains(innovation)) {
        first = getRandomNeuron(fromInput ? Layer::Input : Layer::Hidden);
        second = getRandomNeuron(fromInput ? Layer::Hidden : Layer::Output);
        key = std::make_pair(first->getId(), second->getId());
        innovation = ledger->getOrCreateConnInnovation(key);
        if (++tries > MAX_TRIES) {
            return false;
        }
    }

    auto* conn = new Connection(first, second, innovation, 1.0, false, true);

    connInnovations.insert(innovation);
    connections[innovation ] = conn;
    return true;
}

bool Network::createNeuron() {
    std::vector<Connection*> possibilities;
    for (const auto & [ID, conn] : connections) {
        if (conn->isOriginal() && conn->isEnabled()) {
            possibilities.push_back(conn);
        }
    }
    if (possibilities.empty()) {
        return false;
    }

    auto randomConn = possibilities[HelperMethods::getRandomInt(0, possibilities.size())];
    randomConn->disable();
    int newNeuronId = ledger->getNewNeuronId();
    auto newNeuron = new Neuron(newNeuronId, Layer::Hidden);
    auto connInInnovation = ledger->getOrCreateConnInnovation({randomConn->source->getId(), newNeuronId});
    auto connOutInnovation = ledger->getOrCreateConnInnovation({newNeuronId, randomConn->destination->getId()});


    auto newConnIn = new Connection(randomConn->source, newNeuron, connInInnovation, 1.0, false, true);
    auto newConnOut = new Connection( newNeuron, randomConn->destination, connOutInnovation, randomConn->getWeight(), false, true);

    randomConn->source->addOutgoing(newConnIn);
    newNeuron->addOutgoing(newConnOut);

    connInnovations.insert(connInInnovation);
    connInnovations.insert(connOutInnovation);
//    neuronInnovations.insert(newNeuronId);

    connections[connInInnovation] = (newConnIn);
    connections[connOutInnovation] = (newConnOut);

    hidden.push_back(newNeuron);

    return true;
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

    // Replace checking with try /except?
    try{
        for (auto [ID, connection] : connections) {
            connectionDiff += abs(connection->getWeight() * connection->isEnabled() - network.connections.at(ID)->getWeight() * connection->isEnabled());
        }
    } catch (const std::out_of_range &e) {
        std::cout << "Wrong matching " << std::endl;
        return 0;
    }

    return std::max(10 - connectionDiff, 0.0);
}
