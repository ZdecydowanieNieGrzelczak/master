#include <cassert>
#include "Neuron.h"
#include "Connection.h"


Neuron::Neuron(int id, Layer layer): id{id}, layer{layer} {
    bias = (((rand() % 100) / 100.f) - 0.5f) * 2;
}

// Do not copy connections
Neuron::Neuron(const Neuron &other): bias{other.bias}, layer{other.layer}, id{other.id} {
    currentValue = 0.0;
    outgoing.clear();
}


void Neuron::addOutgoing(int connId) {
    outgoing.push_back(connId);
}

//void Neuron::addIncoming(Connection *conn) {
//    incoming.push_back(conn);
//}

void Neuron::mutate() {
    bias += (((rand() % 100) / 100.f) - 0.5f) * NEURON_MUTATION_FACTOR;
}

//void Neuron::passValue() {
//    for (const auto out : outgoing) {
//        auto val = (currentValue + bias) * out->getWeight();
//        out->destination->receiveValue(val);
//    }
//    currentValue = 0.0;
//}

int Neuron::getId() const {
    return id;
}

Layer Neuron::getLayer() {
    return layer;
}

float Neuron::getFinalValue() {
    // Only for outputs
//    assert(layer == Layer::Output);
    auto value = currentValue + bias;
    currentValue = 0;
    return value;
}

std::ostream &operator<<(std::ostream &os, const Neuron &neuron) {
    os << "Neuron with bias: " << neuron.bias << std::endl;
    os << " And current value: " << neuron.currentValue << std::endl;
    return os;
}

Neuron::~Neuron() {
}

std::vector<int> Neuron::getOutputs() {
    return outgoing;
}



