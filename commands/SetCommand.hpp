#ifndef REDIS_IN_MEMORY_STORE_SET_VALUE_COMMAND_HPP
#define REDIS_IN_MEMORY_STORE_SET_VALUE_COMMAND_HPP

#include "ICommand.hpp"
#include <iostream>
using namespace std;

class SetCommand : public ICommand {
    public : 
        string execute(HashTable & store, const vector<string> &args) override {
            // args[0] == 'SET', args[1] = Key, args[2] = Value
            if (args.size() < 3) {
                return "Error:: wrong number of arugments for 'SET' \r\n";
            }

            const string &key = args[1];
            string value;

            for (size_t i=2; i < args.size(); i++) {
                if (i > 2) value.push_back(' ');
                value += args[i];
            }

            store.set(key, value);
            return "+OK\r\n";
        }
};

#endif