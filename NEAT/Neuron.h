#ifndef MASTER_NEURON_H
#define MASTER_NEURON_H

#include <vector>
#include <ostream>


#pragma once
#include "Connection.fwd.h"
#include "Neuron.fwd.h"

#define NEURON_MUTATION_FACTOR 0.02f

enum Layer {
    Input = 0,
    Hidden = 1,
    Output = 2
};

class Neuron {

public:
    Neuron(int id, Layer layer);
    Neuron(const Neuron &other);
    ~Neuron();

    void addOutgoing(int connId);
//    void addIncoming(Connection *conn);

    void mutate();

    void receiveValue(float val) { this->currentValue += val; }
//    void passValue();

    [[nodiscard]] int getId() const;

    Layer getLayer();

    float getFinalValue();

    friend std::ostream& operator<<(std::ostream& os, const Neuron& conn);

    std::vector<int> getOutputs();

private:
//    std::vector<Connection*> incoming;
    std::vector<int> outgoing;

    float currentValue{0.0};
    float bias{0.0};
    int id;
    Layer layer;


};

#endif //MASTER_NEURON_H
