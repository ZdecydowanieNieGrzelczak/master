#include "Connection.h"

#include <random>

Connection::Connection(Neuron *source, Neuron *destination): source{source}, destination{destination} {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_float_distribution<std::mt19937::result_type> dist6(1,6)
    this->weight = 0.5;
}
