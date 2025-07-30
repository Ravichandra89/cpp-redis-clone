/**
 * Volatile_Random_keys 
 */

#ifndef VOLATILE_RANDROM_EVICTION_STRATEGY_HPP
#define VOLATILE_RANDROM_EVICTION_STRATEGY_HPP

#include "EvictionPolicy.hpp"
#include <unordered_set>

class VolatileRandom : public EvictionPolicy {
        unordered_set<string> volatile_Keys;
    public: 
        void markVolatile(const string& key) {
            volatile_Keys.insert(key);
        }

        void recordAccess(const string& key) override {
            // nothing do to
        }

        bool isRemovable(const string& key) const override {
            return volatile_Keys.count(key) > 0;
        }

        string selectEvictionKey() override {
            if (volatile_Keys.empty()) return "";
            // choose any key from volatile_keys
            int selected = rand() % volatile_Keys.size();
            auto it = volatile_Keys.begin();
            while (--selected) it++;
            string target = *it;
            volatile_Keys.erase(it);

            // Return the choosed eviction key
            return target;
        }
};

#endif