#ifndef REDIS_IN_MEMORY_HASH_TABLE_STORE_HPP
#define REDIS_IN_MEMORY_HASH_TABLE_STORE_HPP

#include <string>
#include <vector>
#include <memory>
#include <mutex>

#include "IHashFunction.hpp"

using namespace std;

struct HashNode
{
    string key;
    string value;
    HashNode *next;
    HashNode(const string &k, const string &v) : key(k), value(v), next(nullptr) {};
};

class HashTable
{
    vector<HashNode *> buckets;
    unique_ptr<IHashFunction> hashFn;
    size_t capacity;
    size_t size_;
    mutable mutex mtx;
    const double loadFactor = 0.75;

    static std::size_t nextPowerOfTwo(std::size_t n)
    {
        if (n == 0)
            return 1;
        --n;
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;

        if (sizeof(std::size_t) > 4)
            n |= n >> 32;
        return n + 1;
    }

    size_t bucketIndex(const string &key) const
    {
        size_t hash = hashFn->hash(key);
        return hash & (capacity - 1);
    }

    void maybeResize()
    {
        // Check if maxLoadFactor exceeds or not
        if ((static_cast<double>(size_ + 1) / capacity) > loadFactor)
        {
            // TODO: Call the Resize method
        }
    }

    void resize(size_t newCapacity)
    {
    }

    void clearAll()
    {
        for (size_t i = 0; i < capacity; i++)
        {
            HashNode *node = buckets[i];
            while (node)
            {
                HashNode *nextNode = node->next;
                delete node;
                node = nextNode;
            }
            buckets[i] = nullptr;
        }
        size_ = 0;
    }

public:
    HashTable(unique_ptr<IHashFunction> hashFn, size_t initialCapacity) : hashFn(move(hashFn)), capacity(initialCapacity), size_(0)
    {
        buckets = vector<HashNode *>(capacity, nullptr);
    };

    // Destructor
    ~HashTable()
    {
        // TODO: Call the clearAll Function
        clearAll();
    }

    // Set Method - (key-value) Insert into HashTable
    void set(const string &key, const string &value)
    {
        lock_guard<mutex> lock(mtx);
        // maybeResize();

        size_t idx = bucketIndex(key);
        HashNode *node = buckets[idx];

        // Insert into existing Node
        while (node)
        {
            if (node->key == key)
            {
                node->value = value;
                return;
            }
            node = node->next;
        }

        // Insert new Node at last
        HashNode *newNode = new HashNode(key, value);
        newNode->next = buckets[idx];
        buckets[idx] = newNode;
        size_++;
    }

    // Get Value Method from hashTable
    pair<bool, string> get(const string &key)
    {
        // put the lock
        lock_guard<mutex> lock(mtx);
        size_t idx = bucketIndex(key);
        HashNode *node = buckets[idx];

        while (node)
        {
            // Check the key
            if (node->key == key)
            {
                return {true, node->value};
            }

            node = node->next;
        }

        return {false, ""};
    }

    // DEL - Delete the Value base on key
    bool del(const string &key)
    {
        lock_guard<std::mutex> lock(mtx);
        size_t idx = bucketIndex(key);
        HashNode *node = buckets[idx];
        HashNode *prev = nullptr;

        while (node)
        {
            if (node->key == key)
            {
                if (prev)
                    prev->next = node->next;
                else
                    buckets[idx] = node->next;
                delete node;
                --size_;
                return true;
            }
            prev = node;
            node = node->next;
        }
        return false;
    }

    struct Stats { std::size_t capacity; std::size_t size; double loadFactor; };
    Stats stats() const {
        std::lock_guard<std::mutex> lock(mtx);
        return {capacity, size_, static_cast<double>(size_) / capacity};
    }
};

#endif