#ifndef REDIS_IN_MEMORY_CACHING_ADAPTER_HPP
#define REDIS_IN_MEMORY_CACHING_ADAPTER_HPP

#include "ICachingCommands.hpp"
#include "../store/RedisDataStore.hpp"
#include "ICommand.hpp"
using namespace std;

class CachingAdapter : public ICommand {
        unique_ptr<ICachingCommands> caching_cmd;
    public: 
        CachingAdapter(unique_ptr<ICachingCommands> cmd) : caching_cmd(move(cmd)) {};

        string execute(class HashTable &/*stub*/, const vector<string> &args) override {

            extern RedisDataStore redisStore;
            return caching_cmd->execute(redistStore, args);
    }

};

#endif