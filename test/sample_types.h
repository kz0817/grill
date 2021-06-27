#pragma once
#include <string>
#include "Integer.h"

static constexpr grill::Integer::block_t MaxBlockValue = 0xffff'ffff'ffff'ffff;

struct binary_op_sample_t {
    const grill::Integer& lhs;
    const grill::Integer& rhs;
    const grill::Integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const binary_op_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs << ", expected: " << s.expected;
        return os;
    }
};

struct binary_mod_op_sample_t {
    const grill::Integer& lhs;
    const grill::Integer& rhs;
    const grill::Integer& mod;
    const grill::Integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const binary_mod_op_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs
           << ", mod: " << s.mod << ", expected: " << s.expected;
        return os;
    }
};

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
