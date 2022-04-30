#ifndef MASTER_CONNECTION_H
#define MASTER_CONNECTION_H

#pragma once
#include "Neuron.fwd.h"
#include "Connection.fwd.h"

#define CONN_MUTATION_FACTOR 0.2

class Connection {
public:
    Connection(Neuron* source, Neuron* destination);
    Connection(Neuron* source, Neuron* destination, double weight);

    void passValue(float val);

    void mutate();
    Neuron* source;
    Neuron* destination;

    double getWeight();

private:

    double weight;

};


#endif //MASTER_CONNECTION_H
