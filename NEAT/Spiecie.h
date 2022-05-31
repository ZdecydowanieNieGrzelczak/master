#ifndef MASTER_SPIECIE_H
#define MASTER_SPIECIE_H


#include "Network.h"

class Spiecie {
public:
    Spiecie(int id, Network* archetype);
    bool testAndAdd(const Network& contestant);
    int getCount() { return specimensCount; }
private:
    Network* archetype;
    int specimensCount{1};
    int id;
};


#endif //MASTER_SPIECIE_H
