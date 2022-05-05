#include <cassert>
#include "Neuron.h"
#include "Connection.h"


Neuron::Neuron(int id, Layer layer): id{id}, layer{layer}, currentValue{0.0} {
     bias += HelperMethods::getRandomInt(-10, 10) * NEURON_INITIAL_FACTOR;
}

Neuron::Neuron(const Neuron &other): id{other.id}, layer{other.layer}, bias{other.bias}, currentValue{0.0}  {

}

void Neuron::mutate() {
    bias += HelperMethods::getRandomInt(-10, 10) * NEURON_MUTATION_FACTOR;
}

int Neuron::getId() const {
    return id;
}

Layer Neuron::getLayer() {
    return layer;
}

float Neuron::getFinalValue() const {
    return currentValue + bias;
}

std::ostream &operator<<(std::ostream &os, const Neuron &neuron) {
    os << "Neuron with bias: " << neuron.bias << std::endl;
    os << " And current value: " << neuron.currentValue << std::endl;
    return os;
}




