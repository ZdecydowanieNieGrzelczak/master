
#include "Spiecie.h"

Spiecie::Spiecie(int id, Network *archetype): id{id}, archetype{archetype} {}

bool Spiecie::testAndAdd(const Network &contestant) {
    if (archetype->getSimilarity(contestant) >= SIMILARITY_THRESHOLD) {
        ++specimensCount;
        return true;
    }
    return false;
}
