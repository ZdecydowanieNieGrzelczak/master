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
    Connection(u_int8_t source, u_int8_t destination, u_int16_t innovationId);
    Connection(const Connection &other) = default;

    void mutate();
    u_int8_t sourceId;
    u_int8_t destinationId;
    u_int16_t innovationId;

    [[nodiscard]] double getWeight() const;
    friend std::ostream& operator<<(std::ostream& os, const Connection& conn);

private:
    double weight;

};



#endif //MASTER_CONNECTION_H
