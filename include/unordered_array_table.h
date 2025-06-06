#pragma once
#include "table_interface.h"

class UnorderedArrayTable : public ITable {
private:
    Container data;

public:
    void insert(const KeyType& key, const ValueType& value) override {
        data.push_back({ key, value });
        operationsCount += data.getOperationsCount();
        data.resetOperationsCount();

        operationsCount += 1; 
    }

    bool contains(const KeyType& key) const override {
        for (size_t i = 0; i < data.size(); ++i) {
            if (equal(data[i].first, key)) {
                return true;
            }
        }
        return false;
    }

    ValueType get(const KeyType& key) const override {
        for (size_t i = 0; i < data.size(); ++i) {
            if (equal(data[i].first, key)) {
                return data[i].second;
            }
        }
        throw std::runtime_error("Key not found");
    }

    void remove(const KeyType& key) override {
        for (size_t i = 0; i < data.size(); ++i) {
            if (equal(data[i].first, key)) {
                data.erase(i);

                operationsCount += data.getOperationsCount();

                data.resetOperationsCount();
                return;
            }
        }
    }
};