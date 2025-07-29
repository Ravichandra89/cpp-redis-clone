#ifndef ALLKEYS_LRU_EVICTION_HPP
#define ALLKEYS_LRU_EVICTION_HPP

#include "EvictionPolicy.hpp"
#include <list>
#include <unordered_map>

/**
 * AllKeysLRU Policy:
 *  - Moves accessed key to the front of the list
 *  - Evicts least recently used (LRU) key from the back
 *  - Insert new key at front
 */

using namespace std;

class AllKeysLRU : public EvictionPolicy {
    list<string> order; 
    unordered_map<string, list<string>::iterator> mp;

public:
    void recordAccess(const string& key) override {
        auto it = mp.find(key);
        if (it != mp.end()) {
            order.erase(it->second);
        }
        order.push_front(key);
        mp[key] = order.begin();
    }

    bool isRemovable(const string& key) const override {
        return true; 
    }

    string selectEvictionKey() override {
        if (order.empty()) return {};

        string victim = order.back();
        order.pop_back();
        mp.erase(victim);
        return victim;
    }
};

#endif
