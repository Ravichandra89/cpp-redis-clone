/**
 * Eviction_Factory_Method
 */
#ifndef EVICTION_FACTORY_HPP
#define EVICTION_FACTORY_HPP

#include "EvictionPolicy.hpp"

#include "AllKeysLFUEviction.hpp"
#include "AllKeysLRUEviction.hpp"
#include "AllKeysRandomEviction.hpp"
#include "VolatileLFUEviction.hpp"
#include "VolatileLFUEviction.hpp"
#include "VolatileRandomEviction.hpp"
#include "VolatileTTLEviction.hpp"
#include "VolatileLRUEviction.hpp"

#include <memory>
#include <string>

using namespace std;

class EvictionFactory {
    public: 
        // method to create Strategies
        unique_ptr<EvictionPolicy> create(const string& policy) {
            if (policy == "allkeys-lru") {
                return make_unique<AllKeysLRU>();
            }
            if (policy == "allkeys-lfu") {
                return make_unique<AllKeysLFU>();
            }
            if (policy == "volatile-lru") {
                return make_unique<VolatileLRU>();
            }
            if (policy == "allkeys-random") {
                return make_unique<AllKeysRandom>();
            }
            if (policy == "volatile-random") {
                return make_unique<VolatileRandom>();
            }
            if (policy == "volatile-ttl") {
                return make_unique<VolatileTTL> ();
            }
            if (policy == "volatile-lfu") {
                return make_unique<VolatileLFU>();
            }

            // default LRU Eviction Policy
            return make_unique<AllKeysLRU>();
        }
};

#endif