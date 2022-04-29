#ifndef MASTER_NEURON_H
#define MASTER_NEURON_H

#pragma once

#include <vector>
#include "Connection.fwd.h"
#include "Neuron.fwd.h"

class Neuron {

public:
    Neuron();

    void addOutgoing(Connection *conn);
    void addIncoming(Connection *conn);

    void mutate();

    void receiveValue(float val) { this->currentValue += val; }
    void passValue();

private:
    std::vector<Connection*> incoming;
    std::vector<Connection*> outgoing;

    float currentValue{0.0};
    float bias;


};

#endif //MASTER_NEURON_H
