#pragma once
#include "table_interface.h"

class OrderedArrayTable : public ITable {
private:
    Container data;

    size_t binarySearch(const KeyType& key) const {
        size_t left = 0, right = data.size();
        while (left < right) {
            size_t mid = left + (right - left) / 2;
            if (equal(data[mid].first, key)) {
                return mid;
            }
            else if (less(data[mid].first, key)) {
                left = mid + 1;
            }
            else {
                right = mid;
            }
        }
        return data.size(); // not found
    }

public:
    void insert(const KeyType& key, const ValueType& value) override {
        size_t pos = 0;
        while (pos < data.size() && less(data[pos].first, key)) {
            ++pos;
        }

        if (pos < data.size() && equal(data[pos].first, key)) {
            data[pos].second = value;
        }
        else {
            data.push_back({ key, value }); // Временная вставка в конец
            operationsCount += data.getOperationsCount();

            // Сдвигаем элемент на правильную позицию
            for (size_t i = data.size() - 1; i > pos; --i) {
                std::swap(data[i], data[i - 1]);
                operationsCount += 3; // swap = 3 move operations
            }
        }
    }

    // Остальные методы используют binarySearch
    bool contains(const KeyType& key) const override {
        return binarySearch(key) != data.size();
    }

    ValueType get(const KeyType& key) const override {
        size_t pos = binarySearch(key);
        if (pos == data.size()) throw std::runtime_error("Key not found");
        return data[pos].second;
    }

    void remove(const KeyType& key) override {
        size_t pos = binarySearch(key);
        if (pos != data.size()) {
            data.erase(pos);
            operationsCount += data.getOperationsCount();
            data.resetOperationsCount();
        }
    }
};