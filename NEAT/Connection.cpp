#include "Connection.h"
#include "Neuron.h"

Connection::Connection(u_int8_t source, u_int8_t destination, u_int16_t innovationId): sourceId{source}, destinationId{destination}, innovationId{innovationId} {
    this->weight = (float)HelperMethods::getRandomInt(-50, 50) / 100.0f;
}

//Connection::Connection(const Connection &other): sourceId{other.sourceId},
//destinationId{other.destinationId}, innovationId{other.innovationId}, weight{other.weight} {
//}

void Connection::mutate() {
    this->weight += ((float)HelperMethods::getRandomInt(-50, 50) / 100.0f) * CONN_MUTATION_FACTOR;
}



double Connection::getWeight() const {
    return weight;
}

std::ostream& operator<<(std::ostream& os, const Connection& conn)
{
    os << "Connection with from: " << conn.sourceId << " to: " << conn.destinationId << std::endl;
    os << "Has weight: " << conn.getWeight() << " and innovation ID: " << conn.innovationId << std::endl;
    return os;
}

