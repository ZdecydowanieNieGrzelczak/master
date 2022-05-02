#include "Connection.h"
#include "Neuron.h"

Connection::Connection(Neuron *source, Neuron *destination): source{source}, destination{destination} {
    std::random_device dev;
    this->weight = (float)HelperMethods::getRandomInt(-50, 50) / 100.0f;
}

Connection::Connection(Neuron *source, Neuron *destination, double weight): source{source}, destination{destination}, weight{weight} {
}

void Connection::mutate() {
    this->weight += ((float)HelperMethods::getRandomInt(-50, 50) / 100.0f) * CONN_MUTATION_FACTOR;
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

