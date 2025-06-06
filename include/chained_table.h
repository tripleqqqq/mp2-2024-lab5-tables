#pragma once
#include "table_interface.h"
#include "list.h"

class ChainedHashTable : public ITable {
private:
    using Bucket = List<KeyValuePair>;
    Bucket* buckets;
    size_t capacity;
    size_t size;

    size_t hashFunction(const KeyType& key) const {
        size_t hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % capacity;
    }

    void rehash() {
        size_t new_capacity = capacity * 2;
        Bucket* new_buckets = new Bucket[new_capacity];

        for (size_t i = 0; i < capacity; ++i) {
            for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it) {
                size_t new_index = hashFunction((*it).first) % new_capacity;
                new_buckets[new_index].push_back(*it);

                operationsCount += 3;
            }
        }

        delete[] buckets;
        buckets = new_buckets;
        capacity = new_capacity;

        operationsCount += capacity + size;
    }

public:

    explicit ChainedHashTable(size_t initial_capacity = 16) : capacity(initial_capacity), size(0) {
        buckets = new Bucket[capacity];

        operationsCount += capacity;
    }

    ~ChainedHashTable() {
        if (buckets) {
            delete[] buckets;
        }
    }

    void insert(const KeyType& key, const ValueType& value) override {
        if (loadFactor() > 0.75) {
            rehash();
        }

        size_t index = hashFunction(key);
        
        operationsCount++;

        for (auto& pair : buckets[index]) {
            if (equal(pair.first, key)) {
                pair.second = value; 

                operationsCount += 2;
                return;
            }

            operationsCount++;
        }

        buckets[index].push_back({ key, value });
        size++;

        operationsCount += 3;
    }

    bool contains(const KeyType& key) const override {
        size_t index = hashFunction(key);

        operationsCount++;

        for (const auto& pair : buckets[index]) {
            if (equal(pair.first, key)) {
                return true;
            }

            operationsCount++;
        }
        return false;
    }

    ValueType get(const KeyType& key) const override {
        size_t index = hashFunction(key);

        operationsCount++;

        for (const auto& pair : buckets[index]) {
            if (equal(pair.first, key)) {
                return pair.second;
            }

            operationsCount++;
        }
        throw std::runtime_error("Key not found");
    }


    void remove(const KeyType& key) override {
        size_t index = hashFunction(key);

        operationsCount++;

        Bucket& bucket = buckets[index];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (equal((*it).first, key)) {
                bucket.erase(it.get_current());
                size--;
                operationsCount += 3;
                return;
            }

            operationsCount++;
        }
    }

    double loadFactor() const {
        return static_cast<double>(size) / capacity;
    }

    size_t getSize() const {
        return size;
    }

    size_t getCapacity() const {
        return capacity;
    }
};