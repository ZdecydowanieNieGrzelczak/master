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
    Connection(int sourceId, int destinationId, int id);
//    Connection(Neuron* source, Neuron* destination, double weight);
    Connection(const Connection& other) = default;

    void mutate();
    int sourceId;
    int destinationId;

    [[nodiscard]] double getWeight() const;

    friend std::ostream& operator<<(std::ostream& os, const Connection& conn);

    int getId();

private:

    double weight;

    int id;
};



#endif //MASTER_CONNECTION_H
