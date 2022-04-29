#ifndef MASTER_CONNECTION_H
#define MASTER_CONNECTION_H

#pragma once
#include "Neuron.fwd.h"
#include "Connection.fwd.h"

#define CONN_MUTATION_FACTOR

class Connection {
public:
    Connection(Neuron* source, Neuron* destination);

    void passValue(float val);

    void mutate();

private:
    Neuron* source;
    Neuron* destination;
    double weight;

};


#endif //MASTER_CONNECTION_H
