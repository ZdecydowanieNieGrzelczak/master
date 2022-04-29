#include "Connection.h"
#include "Neuron.h"
#include <random>

Connection::Connection(Neuron *source, Neuron *destination): source{source}, destination{destination} {
    std::random_device dev;
//    std::mt19937 rng(dev());
    this->weight = (rand() - 0.5) * 2.0;
}

void Connection::passValue(float val) {
    this->destination->receiveValue(val);
}


