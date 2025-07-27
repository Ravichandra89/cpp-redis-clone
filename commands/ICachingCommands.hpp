/**
 * Caching Command Interface - Adapter Design pattern
 */
#ifndef REDIS_INTERFACE_COMMAND_IN_MEMORY_CACHING_COMMAND_INTERFACE_HPP
#define REDIS_INTERFACE_COMMAND_IN_MEMORY_CACHING_COMMAND_INTERFACE_HPP

#include <string>
#include <vector>
using namespace std;

class ICachingCommands {
    public: 
        // Destructor
        virtual ~ICachingCommands() = default;
        virtual string execute(class RedisDataStore & store, const vector<string> &args) = 0;
};

#endif
