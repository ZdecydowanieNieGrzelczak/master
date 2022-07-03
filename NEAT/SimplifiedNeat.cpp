
#include "SimplifiedNeat.h"

SimplifiedNeat::SimplifiedNeat(int inputCount, int outputCount, int id) : Network(inputCount, outputCount, id) {

}

SimplifiedNeat::SimplifiedNeat(const Network &other, int id) : Network(other, id) {

}

//TODO: Add neuron removal
//TODO: Add prunning every nth iterations
std::pair<bool, int> SimplifiedNeat::mutate(int generation) {
    if (generation % PRUNE_EVERY_N == 0) {
        pruneTheNetwork(generation);
    }

    int roll = HelperMethods::getRandomChance();
    if ( roll < WEIGHTS_MUTATION_RATE ) {
        mutateWeights();
    } else if (roll < CONN_TOGGLING_RATE + WEIGHTS_MUTATION_RATE ) {
        toggleConnection(generation);
    }
    if (HelperMethods::getRandomChance() < STRUCTURE_MUTATION_RATE ) {
        return {true, mutateStructure(generation)};
    }
    return {false, 0};
}

bool SimplifiedNeat::mutateStructure(int generation) {
    if (HelperMethods::getRandomChance() > NEURON_CREATION_RATE ) {
        return createNeuron(0);
    } else {
        return createConnection(0);
    }
}

bool SimplifiedNeat::createConnection(int generation) {
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

    auto* conn = new Connection(first, second, innovation, 1.0, false, true, generation);

    connInnovations.insert(innovation);
    connections[innovation ] = conn;
    return true;
}

bool SimplifiedNeat::createNeuron(int generation) {
    std::vector<Connection*> possibilities;
    for (int i = 0; i < originalConnectionsCount; ++i) {
        auto conn = connections[i];
        if (conn->isEnabled()) {
            possibilities.push_back(conn);
        }
    }
    if (possibilities.empty()) {
        return false;
    }



    auto randomConn = possibilities[HelperMethods::getRandomInt(0, possibilities.size())];
    randomConn->disable(generation);
    int newNeuronId = ledger->getNewNeuronId();
    auto newNeuron = new Neuron(newNeuronId, Layer::Hidden);
    auto connInInnovation = ledger->getOrCreateConnInnovation({randomConn->source->getId(), newNeuronId});
    auto connOutInnovation = ledger->getOrCreateConnInnovation({newNeuronId, randomConn->destination->getId()});


    auto newConnIn = new Connection(randomConn->source, newNeuron, connInInnovation, 1.0, false, true, generation);
    auto newConnOut = new Connection( newNeuron, randomConn->destination, connOutInnovation, randomConn->getWeight(), false, true, generation);

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


SimplifiedNeat::~SimplifiedNeat() = default;

void SimplifiedNeat::pruneTheNetwork(int generation) {
    pruneTheConnections(generation);
    pruneNeurons(generation);
}

bool SimplifiedNeat::pruneTheConnections(int generation) {
    std::vector<int> toRemove;
    for (const auto & [ID, conn] : connections) {
        if (!conn->isOriginal() && !conn->isEnabled()) {
            toRemove.push_back(conn->getID());
        }
    }
    if (toRemove.empty()) {
        return false;
    }
    for (auto id : toRemove) {
        deleteConnection(id, false);
    }
    return true;


}

bool SimplifiedNeat::pruneNeurons(int generation) {
    std::vector<int> toRemove;
    for (const auto neuron : hidden) {
        if(neuron->suitableForRemoval(generation)) {
            toRemove.push_back(neuron->getId());
        }

    }
    if (toRemove.empty()) {
        return false;
    }
    for (auto id : toRemove) {
        deleteNeuron(id);
    }
    return true;
}


void SimplifiedNeat::processBestNetwork(int generation) {
    for(int i = 0; i < REMOVE_NEURONS_COUNT; ++i) {
        deleteNeuron(getRandomNeuron(Layer::Hidden)->getId());
    }
}
