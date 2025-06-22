#ifndef REDIS_IN_MEMORY_STORE_I_HASH_FUNCTION_HPP
#define REDIS_IN_MEMORY_STORE_I_HASH_FUNCTION_HPP

#include <string>
#include <cstddef>
using namespace std;

class IHashFunction {
    public : 
        virtual ~IHashFunction() = default;

        // Strategy Design pattern for hashing Algorithms
        virtual size_t hash(const string & key) const = 0;
        
};

#endif