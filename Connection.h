#ifndef MASTER_CONNECTION_H
#define MASTER_CONNECTION_H


#include "Neuron.h"

class Connection {

    Connection(Neuron* source, Neuron* destination);

private:
    Neuron* source;
    Neuron* destination;
    double weight;

};


#endif //MASTER_CONNECTION_H
