#pragma once

namespace grill {
namespace gear {

template<typename T>
bool is_all_zero(const T* blocks, const std::size_t n) {
    for (std::size_t i = 0; i < n; i++) {
        if (blocks[i] != 0)
            return false;
    }
    return true;
}

template<typename T>
void copy(T* dest, const T* src, std::size_t n) {
    for (std::size_t i = 0; i < n; i++)
        dest[i] = src[i];
}

template<typename T>
void fill_zero(T* dest, std::size_t n) {
    for (std::size_t i = 0; i < n; i++)
        dest[i] = 0;
}

} // namespace gear
} // namespace grill

