#pragma once
#include "Integer.h"

namespace grill {
namespace primality {

bool trivial_division(const Integer::block_t n);
bool trivial_division(const Integer& n);
bool fermat_test(const Integer& n);
bool miller_rabin_test(const Integer& n);

} // namespace primality
} // namespace grill
