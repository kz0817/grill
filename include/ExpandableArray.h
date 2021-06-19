#pragma once
#include <stdexcept>

namespace grill {

template<typename T>
class ExpandableArray {
public:
    ExpandableArray(const std::size_t initial_capacity = 8192)
    : capacity(initial_capacity),
      size(0),
      array(new T[this->capacity]) {
    }

    void resize(const std::size_t required_size) {
        if (required_size >= capacity) {
            // TODO: implement
            throw std::logic_error("ExpandableArray::resize: not implemented yet");
        }
        this->size = required_size;
    }

    std::size_t get_size() const {
        return this->size;
    }

    T get(const int idx) {
        return this->array[idx];
    }

    void set(const int idx, T val) {
        this->array[idx] = val;
    }

private:
    std::size_t capacity;
    std::size_t size;
    T* array;
};

} // namespace grill
