#ifndef MASTER_CONNECTION_H
#define MASTER_CONNECTION_H

#pragma once
#include "Neuron.fwd.h"
#include "Connection.fwd.h"
#include "../utils/SharedDefinitions.h"
#include "../utils/HelperMethods.h"

#include <iostream>
#include <random>



class Connection {
public:
    Connection(Neuron* source, Neuron* destination);
    Connection(Neuron* source, Neuron* destination, double weight);

    void mutate();
    Neuron* source;
    Neuron* destination;

    [[nodiscard]] double getWeight() const;

    friend std::ostream& operator<<(std::ostream& os, const Connection& conn);

private:

    double weight;

};



#endif //MASTER_CONNECTION_H
