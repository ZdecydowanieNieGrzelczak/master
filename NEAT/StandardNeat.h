
#ifndef MASTER_STANDARDNEAT_H
#define MASTER_STANDARDNEAT_H


#include "Network.h"

class StandardNeat : public Network {
public:
    StandardNeat(int inputCount, int outputCount, int id);

    StandardNeat(const Network &other, int id);
    StandardNeat(const StandardNeat &other) = delete;
    ~StandardNeat() override;

    std::pair<bool, int> mutate() override;


private:

    bool mutateStructure() override;
    bool createConnection() override;
    bool createNeuron() override;
};


#endif //MASTER_STANDARDNEAT_H
