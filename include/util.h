#pragma once
#include "integer.h"

namespace grill {

class util {
public:
    static integer gcd(const integer& a, const integer& b);
    static std::string to_string(const bool b);
};

} // namespace grill
