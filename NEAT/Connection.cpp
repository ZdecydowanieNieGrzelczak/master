#include "Connection.h"
#include "Neuron.h"
#include <random>

Connection::Connection(Neuron *source, Neuron *destination): source{source}, destination{destination} {
    std::random_device dev;
    this->weight = ((rand() % 100) / 100.f - 0.5) * 2.0;
}

void Connection::passValue(float val) {
    this->destination->receiveValue(val);
}

void Connection::mutate() {
    this->weight += ((rand() % 100) / 100.f - 0.5) * CONN_MUTATION_FACTOR;
}


