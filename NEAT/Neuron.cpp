#include <cassert>
#include "Neuron.h"
#include "Connection.h"


Neuron::Neuron(int id, Layer layer): id{id}, layer{layer} {
    this->bias += ((rand() % 100) / 100.f - 0.5f) * 2;
}

// Do not copy connections
Neuron::Neuron(const Neuron &other): bias{other.bias} {

}


void Neuron::addOutgoing(Connection *conn) {
    outgoing.push_back(conn);
}

void Neuron::addIncoming(Connection *conn) {
    incoming.push_back(conn);
}

void Neuron::mutate() {
    bias += ((rand() % 100) / 100.f - 0.5f) * NEURON_MUTATION_FACTOR;
}

void Neuron::passValue() {
    for (const auto out : outgoing) {
        out->passValue(currentValue + bias);
    }
    this-> currentValue = 0.0;
}

int Neuron::getId() const {
    return id;
}

Layer Neuron::getLayer() {
    return layer;
}

float Neuron::getFinalValue() {
    // Only for outputs
    assert(layer == Layer::Output);
    auto value = currentValue;
    currentValue = 0;
    return value;
}



