#pragma once
#include "Integer.h"

namespace grill {
namespace ntt {

/**
 * Execute NTT(Number Theoretic Tranform) for the given numbers.
 */
void transform(const std::uint64_t* in, std::uint64_t *out, const std::size_t size);
void inverse_transform(const std::uint64_t* in, std::uint64_t *out, const std::size_t size);

} // namespace ntt
} // namespace grill
