#include "Neuron.h"
#include "Connection.h"


Neuron::Neuron() {
    this->bias += ((rand() % 100) / 100.f - 0.5f) * 2;
}


void Neuron::addOutgoing(Connection *conn) {
    this->outgoing.push_back(conn);
}

void Neuron::addIncoming(Connection *conn) {
    this->incoming.push_back(conn);
}

void Neuron::mutate() {
    this->bias += ((rand() % 100) / 100.f - 0.5f) * NEURON_MUTATION_FACTOR;
}

void Neuron::passValue() {
    for (const auto out : this->outgoing) {
        out->passValue(this->currentValue);
    }
    this-> currentValue = 0.0;
}

