#ifndef REDIS_IN_MEMORY_STORE_COMMAND_FACTORY_PATTERN_HPP
#define REDIS_IN_MEMORY_STORE_COMMAND_FACTORY_PATTERN_HPP

#include "SetCommand.hpp"
#include "GetCommand.hpp"
#include "DelCommand.hpp"
#include "ICommand.hpp"

#include <memory>
#include <vector>
#include <unordered_map>
#include "functional"

using namespace std;

class CommandFactory {
        using Creator = function<unique_ptr<ICommand>>();
        unordered_map<string, Creator> registery;

    public : 
        CommandFactory() {
            registery.emplace("SET", [](){
                return make_unique<SetCommand>(); 
            });

            registery.emplace("GET", [](){ 
                return make_unique<GetCommand>();
            });

            registery.emplace("DEL", [](){
                return make_unique<DelCommand>();
            });
        }

        // createMethod Based on command name creating command
            unique_ptr<ICommand> create(const string & name) {
                string uname = name;
                transform(uname.begin(), uname.end(), uname.begin(), ::toupper);
                auto it = registery.find(uname);
                if (it != registery.end()) {
                    return it->second();
                }

                return nullptr;
            }
};

#endif