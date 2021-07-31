#pragma once
#include <cstdint>

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

void mul(uint64_t out[2], const uint64_t in0, const uint64_t in1);

void karatsuba(uint64_t* out, std::size_t n_out,
               const uint64_t* in0, std::size_t num_in0,
               const uint64_t* in1, std::size_t num_in1);

} // namespace gear
} // namespace grill

