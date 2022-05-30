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
    Connection(Neuron* source, Neuron* destination, int innovId, bool original);
    Connection(Neuron* source, Neuron* destination, int innnovId, double weight, bool original, bool enable);
    Connection(const Connection &other);

    void mutate();
    Neuron* source;
    Neuron* destination;

    [[nodiscard]] double getWeight() const;
    [[nodiscard]] bool isEnabled() const;
    [[nodiscard]] bool isOriginal() const;

    void enable() {this->enabled = true; };
    void disable() {this->enabled = false; };
    void toggle() { this->enabled = !this->enabled; };

    friend std::ostream& operator<<(std::ostream& os, const Connection& conn);

private:

    double weight;
    bool enabled = true;
    bool original;
    int innovId;

};



#endif //MASTER_CONNECTION_H
