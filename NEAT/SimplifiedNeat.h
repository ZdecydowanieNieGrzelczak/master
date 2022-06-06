#ifndef MASTER_SIMPLIFIEDNEAT_H
#define MASTER_SIMPLIFIEDNEAT_H


#include "Network.h"

class SimplifiedNeat : public Network {
public:
    SimplifiedNeat(int inputCount, int outputCount, int id);

    SimplifiedNeat(const SimplifiedNeat &other, int id);
    SimplifiedNeat(const SimplifiedNeat &other) = delete;
    ~SimplifiedNeat() override;

    std::pair<bool, int> mutate() override;


private:

    bool mutateStructure() override;
    bool createConnection() override;
    bool createNeuron() override;
};


#endif //MASTER_SIMPLIFIEDNEAT_H
