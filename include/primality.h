#pragma once
#include "integer.h"
#include "constant.h"
#include "util.h"

namespace grill {

class primality {
public:
    static bool trivial_division(const integer::block_t n);
    static bool fermat_test(const integer& n);
};

} // namespace grill
