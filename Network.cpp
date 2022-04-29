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

            auto connection = new Connection(this->inputs[x], this->outputs[y]);
            this->connections.push_back(connection);
            this->inputs[x]->addOutgoing(connection);
            this->outputs[y]->addIncoming(connection);
        }
    }

}

std::vector<float> Network::passThroughNetwork(const std::vector<float> &state) {
    return std::vector<float>();
}
