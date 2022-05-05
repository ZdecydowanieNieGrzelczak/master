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

    void mutate();

    void receiveValue(float val) { this->currentValue += val; }

    int getId() const;

    Layer getLayer();

    float getFinalValue() const;

    friend std::ostream& operator<<(std::ostream& os, const Neuron& conn);


private:

    float currentValue{0.0};
    float bias;
    int id;
    Layer layer;


};

#endif //MASTER_NEURON_H
