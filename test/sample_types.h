#pragma once
#include <string>
#include "Integer.h"

struct Integer_to_uint_sample_t {
    const grill::Integer& n;
    const unsigned long expected;

    friend std::ostream& operator<<(std::ostream& os, const Integer_to_uint_sample_t& s) {
        os << "n: " << s.n << ", expected: " << s.expected;
        return os;
    }
};

struct string_to_Integer_sample_t {
    const std::string s;
    const grill::Integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const string_to_Integer_sample_t& s) {
        os << "s: " << s.s << ", expected: " << s.expected;
        return os;
    }
};
