#include "Connection.h"
#include "Neuron.h"

Connection::Connection(Neuron *source, Neuron *destination, int innovId, bool original): source{source}, destination{destination}
, innovId{innovId}, original{original} {
    this->weight = (float)HelperMethods::getRandomInt(-50, 50) / 100.0f;
}

Connection::Connection(Neuron *source, Neuron *destination, int innovId, double weight, bool original, bool enabled): source{source},
destination{destination}, innovId{innovId}, weight{weight}, original{original}, enabled{enabled} {
}

Connection::Connection(const Connection& other) = default;

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

bool Connection::isEnabled() const {
    return enabled;
}

