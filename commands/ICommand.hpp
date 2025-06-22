#ifndef REDIS_INTERFACE_COMMAND_IN_MEMORY_KEY_VALUE_STORE_HPP
#define REDIS_INTERFACE_COMMAND_IN_MEMORY_KEY_VALUE_STORE_HPP

// Creating Interface class - execute
#include <vector>
#include <string>
#include "../store/HashTable.hpp"
using namespace std;

class ICommand {
    public : 
        virtual ~ICommand() = default;
        virtual string execute(HashTable& hash, const vector<string> &args) = 0;
};

#endif