/**
 * Volatile LFU - Evict those keys which have TTL
 */

#ifndef VOLATILE_LFU_EVICTION_HPP
#define VOLATILE_LFU_EVICTION_HPP

#include "EvictionPolicy.hpp"
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>

class VolatileLFU : public EvictionPolicy {
        unordered_map<string, int> freq;
        multiset<pair<int, string>> tree;
        unordered_set<string> volatile_Keys;

    public: 
        void markVolatile(const string& key) {
            volatile_Keys.insert(key);
        }

        void recordAccess(const string& key) override {
            // If key is not volatile don't need to record
            if (!volatile_Keys.count(key)) return ;
            int prev = freq[key];
            if(prev > 0) tree.erase(tree.find({prev, key}));
            int curr = prev + 1;
            freq[key] = curr;

            // Put updated freq key
            tree.insert({curr, key});
        }

        bool isRemovable(const string& key) const override {
            return volatile_Keys.count(key) > 0;
        }

        string selectEvictionKey() override {
            for (auto it = tree.begin(); it != tree.end(); ++it) {
            if (volatile_Keys.count(it->second)) {
                string victim = it->second;
                tree.erase(it);
                freq.erase(victim);
                volatile_Keys.erase(victim);
                return victim;
            }
        }
        return {};
        }
};

#endif