#include "Connection.h"
#include "Neuron.h"

Connection::Connection(Neuron *source, Neuron *destination): source{source}, destination{destination} {
    std::random_device dev;
    this->weight = (rand() % 100) / 100.0f;
}

void Connection::passValue(float val) {
    this->destination->receiveValue(val);
}

void Connection::mutate() {
    this->weight += ((rand() % 100) / 100.f - 0.5) * CONN_MUTATION_FACTOR;
}

Connection::Connection(Neuron *source, Neuron *destination, double weight): source{source}, destination{destination}, weight{weight} {
}

double Connection::getWeight() const {
    return weight;
}

std::ostream& operator<<(std::ostream& os, const Connection& conn)
{
    os << "Connection with from: " << conn.source->getId() << " to: " << conn.destination->getId() << std::endl;
    os << "Has weight: " << conn.getWeight() << std::endl;
    return os;
}

