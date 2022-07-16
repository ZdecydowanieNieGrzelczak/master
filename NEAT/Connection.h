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
    Connection(Neuron* source, Neuron* destination, const Connection& other);
    Connection(Neuron* source, Neuron* destination, int innovId, double weight, bool original, bool enabled, int generation);
    Connection(const Connection &other) = delete;

    void mutate();
    Neuron* source;
    Neuron* destination;

    [[nodiscard]] double getWeight() const;
    [[nodiscard]] bool isEnabled() const;
    [[nodiscard]] bool isOriginal() const;

    void enable(int counter) { this->enabled = true; lastEnabledAt = counter; };
    void disable(int counter) { this->enabled = false; lastEnabledAt = counter; };
    void toggle(int counter) { this->enabled = !this->enabled; lastEnabledAt = counter; };

    void mergeWithConnection(Connection* other);

    friend std::ostream& operator<<(std::ostream& os, const Connection& conn);
    friend bool operator==(const Connection& left, const Connection& right);

    int getID() const;
    int lastEnabledAt;

private:

    double weight;
    bool enabled = true;
    bool original;
    int innovId;

};



#endif //MASTER_CONNECTION_H
