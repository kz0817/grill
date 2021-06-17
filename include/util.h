#pragma once
#include "integer.h"

namespace grill {

class util {
public:
    static integer gcd(const integer& a, const integer& b);
    static std::string to_string(const bool b);

    /**
     * Convert an integer instance to an unsigned long.
     *
     * The part over the range of an unsigned int is ignored.
     *
     * @param n An integer.
     * @return A converted unsigned int.
     */
    static unsigned long to_uint(const integer& n);

    /**
     * Convert a number string to integer.
     *
     * If a number string begins from 0x, it is handled as a hex decimal number.
     *
     * @param s A number string.
     */
    static integer to_integer(const std::string& s);
};

} // namespace grill
