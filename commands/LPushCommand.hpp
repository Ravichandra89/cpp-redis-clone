/**
 * LPUSH - Push the value at the head of list
 */
#ifndef REDIS_IN_MEMORY_CACHING_LPUSH_COMMAND_HPP
#define REDIS_IN_MEMORY_CACHING_LPUSH_COMMAND_HPP

#include "ICachingCommands.hpp"
#include "../store/RedisDataStore.hpp"
#include <string>

using namespace std;

class lPushCommand : public ICachingCommands {
    public: 
        // Create method 
        static unique_ptr<ICachingCommands> create() {
            return make_unique<lPushCommand>();
        }

        // execute_method
        string execute(class RedisDataStore& redis, const vector<string>& args) override {

            // Args[1] == key && Args[2] == value
            // Check the Arguments 
            if (args.size() < 3) {
                return "-ERR wrong number of arguments for 'LPUSH'\r\n";
            }

            for (size_t t = 2; t < args.size(); t++) {
                redis.lPush(args[1], args[2]);  // passing <key, value>
            }

            return ":" + to_string(redis.llen(args[1])) + "\r\n";
        }
};

#endif