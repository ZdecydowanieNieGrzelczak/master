#ifndef MASTER_CONNECTION_H
#define MASTER_CONNECTION_H

#pragma once
#include "Neuron.fwd.h"
#include "Connection.fwd.h"

#include <iostream>
#include <random>


#define CONN_MUTATION_FACTOR 0.02

class Connection {
public:
    Connection(Neuron* source, Neuron* destination);
    Connection(Neuron* source, Neuron* destination, double weight);

    void passValue(float val);

    void mutate();
    Neuron* source;
    Neuron* destination;

    [[nodiscard]] double getWeight() const;

    friend std::ostream& operator<<(std::ostream& os, const Connection& conn);

private:

    double weight;

};



#endif //MASTER_CONNECTION_H
