#ifndef REDIS_IN_MEMORY_STORE_DEL_COMMAND_HPP
#define REDIS_IN_MEMORY_STORE_DEL_COMMAND_HPP

#include "ICommand.hpp"
#include <string>
using namespace std;

class DelCommand : public ICommand {
    public : 
        string execute(HashTable & store, const vector<string> & args) override {
            // args[0] = 'DEL' args[1] = key
            if (args.size()  < 2) {
                return "-ERR wrong number of arguments for 'DEL'\r\n";
            }

            int deleted = 0;
            for (size_t i=1; i<args.size(); i++) {
                if (store.del(args[i])) {
                    deleted++;
                }
            }
            return ":" + to_string(deleted) + "\r\n";
        }
};

#endif