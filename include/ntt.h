#pragma once
#include "Integer.h"

namespace grill {
namespace ntt {

/**
 * Execute NTT(Number Theoretic Tranform) for the given numbers.
 */
void transform(const std::uint64_t* in, std::uint64_t *out, const std::size_t size);
void inverse_transform(const std::uint64_t* in, std::uint64_t *out, const std::size_t size);

/**
 * Split a number into an array of the specified bits.
 *
 * @param in The number to be split. The least significant chunk first.
 * @param num_in The number of chunks of `in`.
 * @param out An output array containing the split elements.
 * @param num_out The array size of `out`.
 * @param out_unit_bits A bit length of each output element. 
 */
void split(const uint64_t* in, const std::size_t num_in,
           uint64_t* out, const std::size_t num_out, const std::size_t out_unit_bits);

} // namespace ntt
} // namespace grill
