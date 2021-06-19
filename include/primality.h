#pragma once
#include "Integer.h"
#include "constant.h"
#include "util.h"

namespace grill {

class primality {
public:
    static bool trivial_division(const Integer::block_t n);
    static bool trivial_division(const Integer& n);
    static bool fermat_test(const Integer& n);
    static bool miller_rabin_test(const Integer& n);
};

} // namespace grill
