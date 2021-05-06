#pragma once
#include "integer.h"

namespace grill {

struct constant {
    static const wide_int<64> Zero;
    static const wide_int<64> One;
};

} // namespace grill
