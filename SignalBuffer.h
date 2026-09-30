#ifndef SIGNALBUFFER_H
#define SIGNALBUFFER_H

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <utility>

// Owns a dynamically allocated array. The class maintains both size and capacity.
template <typename T>
class SignalBuffer {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
    bool sorted_;

    void updateSortedState() {
        sorted_ = std::is_sorted(data_, data_ + size_);
    }

public:
    explicit SignalBuffer(std::size_t capacity = 50)
        : data_(capacity == 0 ? nullptr : new T[capacity]),
          size_(0), capacity_(capacity), sorted_(true) {}

    // Rule of Three: deep-copy constructor.
    SignalBuffer(const SignalBuffer& other)
        : data_(other.capacity_ == 0 ? nullptr : new T[other.capacity_]),
          size_(other.size_), capacity_(other.capacity_), sorted_(other.sorted_) {
        try {
            for (std::size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        } catch (...) {
            delete[] data_;
            throw;
        }
    }

    // Rule of Three: destructor.
    ~SignalBuffer() {
        delete[] data_;
    }

    // Rule of Three: deep-copy assignment operator.
    SignalBuffer& operator=(const SignalBuffer& other) {
        if (this == &other) {
            return *this;
        }

        T* replacement = other.capacity_ == 0 ? nullptr : new T[other.capacity_];
        try {
            for (std::size_t i = 0; i < other.size_; ++i) {
                replacement[i] = other.data_[i];
            }
        } catch (...) {
            delete[] replacement;
            throw;
        }

        delete[] data_;
        data_ = replacement;
        size_ = other.size_;
        capacity_ = other.capacity_;
        sorted_ = other.sorted_;
        return *this;
    }

    bool append(const T& value) {
        if (size_ >= capacity_) {
            return false;
        }

        // If appending breaks ascending order, update the status.
        if (size_ > 0 && value < data_[size_ - 1]) {
            sorted_ = false;
        }
        data_[size_++] = value;
        return true;
    }

    SignalBuffer& operator+=(const T& value) {
        if (!append(value)) {
            throw std::length_error("Buffer capacity exceeded.");
        }
        return *this;
    }

    void sortData() {
        std::sort(data_, data_ + size_);
        sorted_ = true;
    }

    bool isSorted() const { return sorted_; }
    bool empty() const { return size_ == 0; }
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }

    T& operator[](std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("SignalBuffer index is out of range.");
        }
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("SignalBuffer index is out of range.");
        }
        return data_[index];
    }

    // Read-only pointer access for the template search functions.
    const T* data() const { return data_; }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const SignalBuffer<U>& buffer);
};

template <typename U>
std::ostream& operator<<(std::ostream& out, const SignalBuffer<U>& buffer) {
    const std::size_t shown = std::min<std::size_t>(buffer.size_, 25);
    for (std::size_t i = 0; i < shown; ++i) {
        if (i > 0) out << ' ';
        out << buffer.data_[i];
    }
    if (buffer.size_ > 25) {
        if (shown > 0) out << '\n';
        out << "... additional values not shown";
    }
    return out;
}

#endif
