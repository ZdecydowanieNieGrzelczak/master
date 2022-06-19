#ifndef MASTER_HELPERMETHODS_H
#define MASTER_HELPERMETHODS_H

//old line: #include<io.h>
#ifdef _WIN32
#include <io.h>
#elif __linux__
#include <inttypes.h>
    #include <unistd.h>
    #define __int64 int64_t
    #define _close close
    #define _read read
    #define _lseek64 lseek64
    #define _O_RDONLY O_RDONLY
    #define _open open
    #define _lseeki64 lseek64
    #define _lseek lseek
    #define stricmp strcasecmp
#endif
//SAF_Handle.cpp line:458 old line:INFILE = _open(infilename, _O_RDONLY | _O_BINARY);
#ifdef __linux__
INFILE = _open(infilename, _O_RDONLY);
#elif
INFILE = _open(infilename, _O_RDONLY | _O_BINARY);
#endif

#include <iostream>
#include <vector>
#include <random>


class HelperMethods {
public:

    template <typename T> static void printVector(const std::vector<T>& vec);
    template <typename T> static void printPointerVector(const std::vector<T*>& vec);

    static bool getCoinFlip();
    static int getRandomChance();
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
