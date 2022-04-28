#include "Network.h"

Network::Network(int inputCount, int outputCount) {
    for (int x = 0; x < inputCount; ++x) {
        this->inputs.emplace_back();
    }
    for (int x = 0; x < outputCount; ++x) {
        this->outputs.emplace_back();
    }

    for (int x = 0; x < inputCount; ++x) {
        for (int y = 0; y < outputCount; ++y) {
            this->connections.emplace_back(&this->inputs[x], &this->outputs[y]);
        }
    }

}
