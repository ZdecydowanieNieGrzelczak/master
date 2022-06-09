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


void Neuron::addOutgoing(Connection *conn) {
    outgoing.push_back(conn);
}

//void Neuron::addIncoming(Connection *conn) {
//    incoming.push_back(conn);
//}

void Neuron::mutate() {
    bias += (((rand() % 100) / 100.f) - 0.5f) * NEURON_MUTATION_FACTOR;
}

void Neuron::passValue() {
    for (const auto & out : outgoing) {
        if (out->isEnabled()) {
            double weight = out->getWeight();
            auto val = (currentValue + bias) * weight;
            out->destination->receiveValue(val);
        }
    }
    currentValue = 0.0;
}

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

bool Neuron::removeFromOutgoing(Connection* conn) {
    outgoing.erase(std::remove(outgoing.begin(), outgoing.end(), conn), outgoing.end());
    return outgoing.empty();
}

bool Neuron::suitableForRemoval(int generation) {
    return std::count_if(outgoing.begin(), outgoing.end(), [](Connection *out){ return !out->isEnabled(); }) == outgoing.size();
}

bool operator==(const Neuron &left, const Neuron &right) {
    return left.id == right.id;
}



