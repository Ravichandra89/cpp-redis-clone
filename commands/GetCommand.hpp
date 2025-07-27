#ifndef REDIS_IN_MEMORY_STORE_GET_COMMAND_HPP
#define REDIS_IN_MEMORY_STORE_GET_COMMAND_HPP

#include "ICommand.hpp"
#include <string>
using namespace std;

class GetCommand : public ICommand {
    public : 
        string execute(HashTable & store, const vector<string> &args) override {
            // args[0] == 'GET' args[1] = key
            if (args.size() < 2) {
                return "-ERR wrong number of arguments passed for 'GET' \r\n";
            }

            const string & key = args[1];
            auto result = store.get(key);
            if (!result.first) {
                return "$-1\r\n";
            }

            const string & value = result.second;
            return "$" + to_string(value.size()) + "\r\n" + value + "\r\n";
        }

        static std::unique_ptr<ICommand> create() { return std::make_unique<GetCommand>(); }
};

#endif