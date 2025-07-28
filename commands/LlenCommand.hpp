/**
 * Return the length of list stored at particular key
 */
#ifndef LLEN_COMMAND_HPP
#define LLEN_COMMAND_HPP

#include "../store/RedisDataStore.hpp"
#include "ICachingCommands.hpp"

class lLenCommand : public ICachingCommands {
public:

    static unique_ptr<ICachingCommands> create() {
        return make_unique<lLenCommand> ();
    }

    string execute(class RedisDataStore& store, const vector<string> &args) override {
        if (args.size() != 2) return "-ERR wrong number of arguments for 'LLEN'";
        size_t len = store.llen(args[1]);
        return ":" + to_string(len) + "";
    }

};

class lRangeCommand : public ICachingCommands {
public: 
    static unique_ptr<ICachingCommands> create() {
        return make_unique<lRangeCommand>();
    }

    string execute(class RedisDataStore& store, const vector<string> &args) override {
        if (args.size() != 4) return "-ERR wrong number of arguments for 'LRANGE'";

        int start = stoi(args[2]);
        int stop  = stoi(args[3]);
        auto list = store.lrange(args[1], start, stop);
        string resp = "*" + to_string(list.size()) + "";

        for (auto & ele : list) {
            resp += "$" + to_string(ele.size()) + "" + ele + "";
        }

        return resp;
    }
};

#endif