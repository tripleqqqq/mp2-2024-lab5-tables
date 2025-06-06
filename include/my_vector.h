#pragma once
#include <stdexcept> 
#include <utility>   

template <typename T>
class myVector {

private:

    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
    mutable size_t operationsCount_ = 0; 

    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);

            operationsCount_ += 2; 
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;

        operationsCount_ += 3; 
    }

public:

    myVector() = default;

    explicit myVector(size_t size, const T& value = T())
        : size_(size), capacity_(size) {
        data_ = new T[size];
        for (size_t i = 0; i < size; ++i) {
            data_[i] = value;

            operationsCount_++;
        }
        operationsCount_ += 2; 
    }


    ~myVector() { delete[] data_; }

    myVector(const myVector& other)
        : size_(other.size_), capacity_(other.capacity_) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];

            operationsCount_++;
        }
    }

    myVector(myVector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    myVector& operator=(const myVector& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];

                operationsCount_++;
            }
        }
        return *this;
    }

    T& operator[](size_t index) {

        operationsCount_++;

        return data_[index];
    }

    const T& operator[](size_t index) const {

        operationsCount_++;

        return data_[index];
    }

    T& at(size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");

        operationsCount_++;

        return data_[index];
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;

        operationsCount_ += 2; 
    }

    void erase(size_t index) {
        if (index >= size_) return;
        for (size_t i = index; i < size_ - 1; ++i) {
            data_[i] = std::move(data_[i + 1]);

            operationsCount_++;
        }
        size_--;
        operationsCount_ += 2;
    }

    size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    size_t getOperationsCount() const noexcept { return operationsCount_; }
    void resetOperationsCount() const noexcept { operationsCount_ = 0; }
};
