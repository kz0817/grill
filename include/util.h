#pragma once
#include "Integer.h"

namespace grill {

class util {
public:
    static Integer gcd(const Integer& a, const Integer& b);
    static std::string to_string(const bool b);

    /**
     * Convert an Integer instance to an unsigned long.
     *
     * The part over the range of an unsigned int is ignored.
     *
     * @param n An Integer.
     * @return A converted unsigned int.
     */
    static unsigned long to_uint(const Integer& n);

    /**
     * Convert a number string to Integer.
     *
     * If a number string begins from 0x, it is handled as a hex decimal number.
     *
     * @param s A number string.
     */
    static Integer to_Integer(const std::string& s);
};

} // namespace grill
