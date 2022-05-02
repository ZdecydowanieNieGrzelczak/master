#include "Connection.h"
#include "Neuron.h"

Connection::Connection(int sourceId, int destinationId, int id): sourceId{sourceId}, destinationId{destinationId}, id{id} {
    std::random_device dev;
    this->weight = (float)HelperMethods::getRandomInt(-50, 50) / 100.0f;
}


void Connection::mutate() {
    this->weight += ((float)HelperMethods::getRandomInt(-50, 50) / 100.0f) * CONN_MUTATION_FACTOR;
}

double Connection::getWeight() const {
    return weight;
}

std::ostream& operator<<(std::ostream& os, const Connection& conn)
{
    os << "Connection with from: " << conn.sourceId << " to: " << conn.destinationId << std::endl;
    os << "Has weight: " << conn.getWeight() << std::endl;
    return os;
}

int Connection::getId() {
    return id;
}


