/**
 * Volatile Least Recently Used Eviction :
 *  - Remove those keys which has TTL and Least Recently Used.
 *  - volatileKeys (Insert those keys which are volatile)
 *  - orders (Doubly Linked list)
 * 
 */
#ifndef VOLATILE_LEAST_RECENTLY_USED_EVICTION_HPP;
#define VOLATILE_LEAST_RECENTLY_USED_EVICTION_HPP;

#include "EvictionPolicy.hpp"
#include <list>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class VolatileLRU : public EvictionPolicy {
        list<string> order;
        unordered_map<string, list<string>::iterator> mp;
        unordered_set<string> volatile_Keys;

    public: 
        void markVolatile(const string& key) {
            volatile_Keys.insert(key);
        }

        void recordAccess(const string& key) override {
            if (volatile_Keys.count(key) == 0) return;
            auto it = mp.find(key);
            if (it != mp.end()) order.erase(it->second);
            order.push_front(key);
            mp[key] = order.begin();
        }
        
};

#endif