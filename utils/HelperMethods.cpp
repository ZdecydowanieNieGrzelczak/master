#include "HelperMethods.h"


bool HelperMethods::getCoinFlip() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> coinFlip(0,1);
    return coinFlip(rng);
}

int HelperMethods::getRandomInt(int lower, int upper) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randInt(lower,upper);
    return randInt(rng);
}