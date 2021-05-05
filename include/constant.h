#pragma once
#include "integer.h"

namespace grill {

struct constant {
    static const wide_int<64> zero;
    static const wide_int<64> one;
};

} // namespace grill
