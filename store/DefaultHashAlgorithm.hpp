#ifndef REDIS_IN_MEMORY_STORE_HASHING_ALGORITHM_DEFAULT_HPP
#define REDIS_IN_MEMORY_STORE_HASHING_ALGORITHM_DEFAULT_HPP

#include "IHashFunction.hpp"
#include <iostream>
using namespace std;

// Default hash: DJB2 algorithm
class DefaultHash : public IHashFunction {
public:
    std::size_t hash(const std::string &key) const override {
        std::size_t h = 5381;
        for (unsigned char c : key) {
            h = ((h << 5) + h) + c; 
        }
        return h;
    }
};

#endif