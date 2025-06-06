#pragma once
#include <string>
#include <ostream>
#include <typeinfo>
#include "polinoms.h"
#include "my_vector.h"

class ITable {
public:

    using KeyType = std::string;
    using ValueType = Polynom;
    using KeyValuePair = std::pair<KeyType, ValueType>;
    using Container = myVector<KeyValuePair>;

    virtual ~ITable() = default;

    virtual void insert(const KeyType& key, const ValueType& value) = 0;
    virtual bool contains(const KeyType& key) const = 0;
    virtual ValueType get(const KeyType& key) const = 0;
    virtual void remove(const KeyType& key) = 0;

    virtual void logOperation(const std::string& opName, std::ostream& out = std::cout) const {
        out << "[" << typeid(*this).name() << "] "
            << opName << " operations: " << operationsCount << "\n";
        resetOperationsCount();
    }

    virtual size_t getOperationsCount() const noexcept {
        return operationsCount;
    }

    virtual void resetOperationsCount() const noexcept {
        operationsCount = 0;
    }

protected:
    virtual bool less(const KeyType& a, const KeyType& b) const {
        operationsCount++;
        return a < b;
    }

    virtual bool lessOrEqual(const KeyType& a, const KeyType& b) const {
        operationsCount++;
        return a <= b;
    }

    virtual bool greater(const KeyType& a, const KeyType& b) const {
        operationsCount++;
        return a > b;
    }

    virtual bool greaterOrEqual(const KeyType& a, const KeyType& b) const {
        operationsCount++;
        return a >= b;
    }

    virtual bool equal(const KeyType& a, const KeyType& b) const {
        operationsCount++;
        return a == b;
    }

    virtual bool notEqual(const KeyType& a, const KeyType& b) const {
        operationsCount++;
        return a != b;
    }

    mutable size_t operationsCount = 0;
};
