#ifndef MASTER_NEURON_H
#define MASTER_NEURON_H

#include <vector>


#pragma once
#include "Connection.fwd.h"
#include "Neuron.fwd.h"

#define NEURON_MUTATION_FACTOR 0.2f

enum Layer {
    Input,
    Hidden,
    Output
};

class Neuron {

public:
    Neuron(int id, Layer layer);
    Neuron(const Neuron &other);

    void addOutgoing(Connection *conn);
    void addIncoming(Connection *conn);

    void mutate();

    void receiveValue(float val) { this->currentValue += val; }
    void passValue();

    int getId() const;

    Layer getLayer();

    float getFinalValue();

private:
    std::vector<Connection*> incoming;
    std::vector<Connection*> outgoing;

    float currentValue{0.0};
    float bias;
    int id;
    Layer layer;


};

#endif //MASTER_NEURON_H
