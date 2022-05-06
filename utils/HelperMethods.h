#ifndef MASTER_HELPERMETHODS_H
#define MASTER_HELPERMETHODS_H

#include <iostream>
#include <vector>
#include <random>

class HelperMethods {
public:

    template <typename T> static void printVector(const std::vector<T>& vec);
    template <typename T> static void printPointerVector(const std::vector<T*>& vec);

    static bool getCoinFlip();
    static int getRandomInt(int lower, int upper);
    static bool nameTest(const std::string &name);
};

template<typename T>
void HelperMethods::printVector(const std::vector<T> &vec) {
    for (auto member: vec) {
        std::cout << member << " ";
    }
    std::cout << std::endl;
}

template<typename T>
void HelperMethods::printPointerVector(const std::vector<T*> &vec) {
    for (auto member: vec) {
        std::cout << *member << " ";
    }
    std::cout << std::endl;
}


#endif //MASTER_HELPERMETHODS_H
