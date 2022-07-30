#ifndef MASTER_NEURON_H
#define MASTER_NEURON_H

#include <vector>
#include <ostream>


#pragma once
#include "Connection.fwd.h"
#include "Neuron.fwd.h"


enum Layer {
    Input,
    Hidden,
    Output
};

class Neuron {

public:
    Neuron(int id, Layer layer);
    Neuron(const Neuron &other);
    Neuron(const float bias, const int id, const Layer layer);

    ~Neuron();

    void addOutgoing(Connection *conn);
    bool removeFromOutgoing(Connection *conn);

    void mutate();

    void receiveValue(float val) { this->currentValue += val; }
    void passValue();
    bool suitableForRemoval(int generation);

    int getId() const;

    Layer getLayer();

    float getFinalValue();

    friend std::ostream& operator<<(std::ostream& os, const Neuron& conn);
    friend bool operator==(const Neuron& left, const Neuron& right);


    float getBias() { return bias; }


    void mergeWithNeuron(Neuron *pNeuron);

private:
//    std::vector<Connection*> incoming;
    std::vector<Connection*> outgoing;

    float currentValue{0.0};
    float bias{0.0};
    int id;
    Layer layer;
    bool merged{false};


};

#endif //MASTER_NEURON_H
