#pragma once
#include <string>
#include "integer.h"

struct integer_to_uint_sample_t {
    const grill::integer& n;
    const unsigned long expected;

    friend std::ostream& operator<<(std::ostream& os, const integer_to_uint_sample_t& s) {
        os << "n: " << s.n << ", expected: " << s.expected;
        return os;
    }
};

struct string_to_integer_sample_t {
    const std::string s;
    const grill::integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const string_to_integer_sample_t& s) {
        os << "s: " << s.s << ", expected: " << s.expected;
        return os;
    }
};
