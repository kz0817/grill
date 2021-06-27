#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <vector>
#include "Integer.h"
#include "constant.h"
#include "sample_types.h"
#include "test-funcs.h"
#include "util.h"

using namespace grill;

struct cmp_sample_t {
    const Integer& lhs;
    const Integer& rhs;
    bool expected;

    friend std::ostream& operator<<(std::ostream& os, const cmp_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs <<
              ", expected: " << util::to_string(s.expected);
        return os;
    }
};

struct unary_op_sample_t {
    const Integer& n;
    const Integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const unary_op_sample_t& s) {
        os << "lhs: " << ", expected: " << s.expected;
        return os;
    }
};

struct Integer_and_int_sample_t {
    const Integer& lhs;
    int rhs;
    const Integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const Integer_and_int_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs << ", expected: " << s.expected;
        return os;
    }
};

struct Integer_to_int_sample_t {
    const Integer& n;
    const int expected;

    friend std::ostream& operator<<(std::ostream& os, const Integer_to_int_sample_t& s) {
        os << "n: " << s.n << ", expected: " << s.expected;
        return os;
    }
};

struct int_to_Integer_sample_t {
    const int n;
    const Integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const int_to_Integer_sample_t& s) {
        os << "n: " << s.n << ", expected: " << s.expected;
        return os;
    }
};

struct Integer_int_to_bool_sample_t {
    const Integer& lhs;
    const int rhs;
    const bool expected;

    friend std::ostream& operator<<(std::ostream& os, const Integer_int_to_bool_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs
           << ", expected: " << util::to_string(s.expected);
        return os;
    }
};

struct Integer_to_bool_sample_t {
    const Integer& n;
    const bool expected;

    friend std::ostream& operator<<(std::ostream& os, const Integer_to_bool_sample_t& s) {
        os << "n: " << s.n << ", expected: " << util::to_string(s.expected);
        return os;
    }
};

BOOST_AUTO_TEST_SUITE(test_suite_Integer)

static struct cast_string_sample_t {
    const Integer& n;
    std::string expected;

    friend std::ostream& operator<<(std::ostream& os, const cast_string_sample_t& s) {
        os << "n.num_blocks: " << s.n.get_num_blocks() << ", expected: " << s.expected;
        return os;
    }
} cast_string_samples[] {
    {WideInt<64>(0x0123456789abcdef), "0123456789abcdef"},
    {WideInt<64>(0xfedcba9876543210), "fedcba9876543210"},
    {WideInt<128>(1), "0000000000000000" "0000000000000001"},
    {WideInt<128>({0x0123456789abcdef, 0x11223344aabbccdd}),
                   "0123456789abcdef" "11223344aabbccdd"},
};

BOOST_DATA_TEST_CASE(cast_string, cast_string_samples)
{
    BOOST_TEST(static_cast<std::string>(sample.n) == sample.expected);
}

//
// += and + operator
//
static binary_op_sample_t add_operator_samples[] = {
    {
        WideInt<64>(10),
        WideInt<64>(3),
        WideInt<64>(13)
    }, {
        WideInt<64>(MaxBlockValue),
        WideInt<64>(1),
        constant::Zero
    },

    {
        WideInt<256>({1, 2, 3, 4}),
        WideInt<128>({10, 20}),
        WideInt<256>({1, 2, 13, 24})
    }, {
        WideInt<256>({1, MaxBlockValue, MaxBlockValue, MaxBlockValue}),
        WideInt<128>({1, 2}),
        WideInt<256>({2, 0, 1, 1})
    },

    {
        WideInt<128>({1, 2}),
        WideInt<256>({10, 20, 30, 40}),
        WideInt<128>({31, 42})
    }, {
        WideInt<128>({1, MaxBlockValue}),
        WideInt<256>({1, 2, 3, 4}),
        WideInt<128>({5, 3})
    },
};

BOOST_DATA_TEST_CASE(add_unary_operator, add_operator_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n += sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

BOOST_DATA_TEST_CASE(add_binary_operator, add_operator_samples)
{
    BOOST_TEST((sample.lhs + sample.rhs) == sample.expected);
}

static binary_op_sample_t mul_operator_samples[] {
    {WideInt<64>(0), WideInt<64>(1),  WideInt<64>(0)},
    {WideInt<64>(1), WideInt<64>(1),  WideInt<64>(1)},
    {WideInt<64>(1), WideInt<64>(2),  WideInt<64>(2)},
    {WideInt<64>(3), WideInt<64>(10), WideInt<64>(30)},
    {WideInt<64>(0xffff'ffff),  WideInt<64>(0xffff'ffff), WideInt<64>(0xffff'fffe'0000'0001)},
    {WideInt<64>(0x1'0000'0001),  WideInt<64>(0x2'0000'0003), WideInt<64>(0x5'0000'0003)},

    {WideInt<64>(0x1234'5678'ffff'ffff),  WideInt<64>(0x2468'ace0'ffff'ffff),
     WideInt<64>(0xc962'fca6'0000'0001)},

    {WideInt<128>(0x0001'0002'0000'0003),  WideInt<64>(0x0002'0003'0000'0001),
     WideInt<128>({0x2'0007'0006, 0x7'000b'0000'0003})},

    {WideInt<128>(0x1234'5678'ffff'ffff),  WideInt<64>(0x2468'ace0'ffff'ffff),
     WideInt<128>({0x296'cdb7'4f41'4c58, 0xc962'fca6'0000'0001})},

    {WideInt<128>({0x1234'5678'9abc'def0, 0x1234'5678'9abc'def0}),
     WideInt<128>({0x1234'5678'9abc'def0, 0x1234'5678'9abc'def0}),
     WideInt<128>({0x4d0f'77fe'1940'eedc, 0xa5e2'0890'f2a5'2100})},

    {WideInt<256>({0x1234'5678'9abc'def0, 0x1234'5678'9abc'def0}),
     WideInt<256>({0x1234'5678'9abc'def0, 0x1234'5678'9abc'def0}),
     WideInt<256>({0x014b'66dc'33f6'acdc, 0xa878'd649'5a92'7ab9,
                    0x4d0f'77fe'1940'eedc, 0xa5e2'0890'f2a5'2100})},
};

BOOST_DATA_TEST_CASE(mul_unary_operator, mul_operator_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n *= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

BOOST_DATA_TEST_CASE(mul_binary_opreator, mul_operator_samples)
{
    BOOST_TEST((sample.lhs * sample.rhs) == sample.expected);
}

BOOST_AUTO_TEST_CASE(substitution)
{
    WideInt<256> n({1, 2, 3, 4});
    n = 5;

    const Integer::block_t expected[] = {5, 0, 0, 0};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(move_substitution)
{
    WideInt<256> src({1, 2, 3, 4});
    const Integer n = std::move(src);

    const Integer::block_t expected[] = {4, 3, 2, 1};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

static cmp_sample_t equal_samples[] {
    {WideInt<64>(7),  WideInt<64>(7),  true},
    {WideInt<64>(7),  WideInt<64>(9),  false},
    {WideInt<64>(7),  WideInt<64>(5),  false},
    {WideInt<256>(7), WideInt<64>(7),  true},
    {WideInt<256>(7), WideInt<64>(9),  false},
    {WideInt<256>(7), WideInt<64>(5),  false},
    {WideInt<64>(7),  WideInt<256>(7), true},
    {WideInt<64>(7),  WideInt<256>(9), false},
    {WideInt<64>(7),  WideInt<256>(5), false},
};

BOOST_DATA_TEST_CASE(equal, equal_samples)
{
    BOOST_TEST((sample.lhs == sample.rhs) == sample.expected);
}

static cmp_sample_t not_equal_samples[] {
    {WideInt<64>(7),  WideInt<64>(7),  false},
    {WideInt<64>(7),  WideInt<64>(9),  true},
    {WideInt<64>(7),  WideInt<64>(5),  true},
    {WideInt<256>(7), WideInt<64>(7),  false},
    {WideInt<256>(7), WideInt<64>(9),  true},
    {WideInt<256>(7), WideInt<64>(5),  true},
    {WideInt<64>(7),  WideInt<256>(7), false},
    {WideInt<64>(7),  WideInt<256>(9), true},
    {WideInt<64>(7),  WideInt<256>(5), true},
};

BOOST_DATA_TEST_CASE(not_equal, not_equal_samples)
{
    BOOST_TEST((sample.lhs != sample.rhs) == sample.expected);
}

static cmp_sample_t gt_eq_samples[] {
    {WideInt<64>(7),  WideInt<64>(7),  true},
    {WideInt<64>(7),  WideInt<64>(9),  false},
    {WideInt<64>(7),  WideInt<64>(5),  true},
    {WideInt<256>(7), WideInt<64>(7),  true},
    {WideInt<256>(7), WideInt<64>(9),  false},
    {WideInt<256>(7), WideInt<64>(5),  true},
    {WideInt<64>(7),  WideInt<256>(7), true},
    {WideInt<64>(7),  WideInt<256>(9), false},
    {WideInt<64>(7),  WideInt<256>(5), true},
};

BOOST_DATA_TEST_CASE(gt_eq, gt_eq_samples)
{
    BOOST_TEST((sample.lhs >= sample.rhs) == sample.expected);
}

static cmp_sample_t lt_eq_samples[] {
    {WideInt<64>(7),  WideInt<64>(7),  true},
    {WideInt<64>(7),  WideInt<64>(9),  true},
    {WideInt<64>(7),  WideInt<64>(5),  false},
    {WideInt<256>(7), WideInt<64>(7),  true},
    {WideInt<256>(7), WideInt<64>(9),  true},
    {WideInt<256>(7), WideInt<64>(5),  false},
    {WideInt<64>(7),  WideInt<256>(7), true},
    {WideInt<64>(7),  WideInt<256>(9), true},
    {WideInt<64>(7),  WideInt<256>(5), false},
};

BOOST_DATA_TEST_CASE(lt_eq, lt_eq_samples)
{
    BOOST_TEST((sample.lhs <= sample.rhs) == sample.expected);
}

static binary_op_sample_t bitwise_and_samples[] {
    {WideInt<64>(0xea),  WideInt<64>(0xcc),  WideInt<64>(0xc8)},
    {WideInt<256>(0xea), WideInt<64>(0xcc),  WideInt<256>(0xc8)},
    {WideInt<64>(0xea),  WideInt<256>(0xcc), WideInt<256>(0xc8)},
};

BOOST_DATA_TEST_CASE(bitwise_and, bitwise_and_samples)
{
    const Integer result = sample.lhs & sample.rhs;
    BOOST_TEST(result == sample.expected);
    BOOST_TEST(result.get_num_blocks() == sample.expected.get_num_blocks());
}

static Integer_and_int_sample_t left_shift_samples[] {
    {WideInt<64>(0x40002000100080a5), 0, WideInt<64>(0x40002000100080a5)},
    {WideInt<64>(0x40002000100080a5), 1, WideInt<64>(0x800040002001014a)},
    {WideInt<128>(0x40002000100080a5), 8, WideInt<128>({0x40, 0x002000100080a500})},
    {WideInt<128>(0x40002000100080a5), 64, WideInt<128>({0x40002000100080a5, 0})},
    {WideInt<128>(0x40002000100080a5), 127, WideInt<128>({0x8000000000000000, 0})},
    {WideInt<128>(0x40002000100080a5), 128, WideInt<128>({0, 0})},
    {WideInt<128>(0x40002000100080a5), 129, WideInt<128>({0, 0})},
    {WideInt<128>(0x40002000100080a5), 256, WideInt<128>({0, 0})},
};

BOOST_DATA_TEST_CASE(left_shift, left_shift_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n <<= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

static Integer_and_int_sample_t right_shift_samples[] {
    {WideInt<64>(0x40002000100080a5), 0, WideInt<64>(0x40002000100080a5)},
    {WideInt<64>(0x800040002001014a), 1, WideInt<64>(0x40002000100080a5)},
    {WideInt<128>({0x40, 0x002000100080a500}), 8, WideInt<128>(0x40002000100080a5)},
    {WideInt<128>({0x40002000100080a5, 0}),   64, WideInt<128>(0x40002000100080a5)},
    {WideInt<128>({0x8fff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 127, WideInt<128>(1)},
    {WideInt<128>({0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 128, WideInt<128>(0)},
    {WideInt<128>({0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 129, WideInt<128>(0)},
    {WideInt<128>({0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 256, WideInt<128>(0)},
};

BOOST_DATA_TEST_CASE(right_shift, right_shift_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n >>= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

static unary_op_sample_t prefix_increment_samples[] {
    {WideInt<64>(0), WideInt<64>(1)},
    {WideInt<64>(1), WideInt<64>(2)},
    {WideInt<64>(0xffff'ffff'ffff'ffff), WideInt<64>(0)},
};

BOOST_DATA_TEST_CASE(prefix_increment, prefix_increment_samples)
{
    Integer n = sample.n;
    BOOST_TEST(++n == sample.expected);
}

static Integer_to_int_sample_t most_significant_active_bit_samples[] {
    {WideInt<64>(0), 0},
    {WideInt<64>(1), 1},
    {WideInt<64>(2), 2},
    {WideInt<64>(3), 2},
    {WideInt<64>(0xa5), 8},
    {WideInt<64>(0x4123'4567'89ab'cdef), 63},
    {WideInt<64>(0x8123'4567'89ab'cdef), 64},
    {WideInt<128>({0, 0x8123'4567'89ab'cdef}), 64},
    {WideInt<128>({1, 0x8123'4567'89ab'cdef}), 65},
    {WideInt<128>({0xa0, 0x8123'4567'89ab'cdef}), 72},
};

BOOST_DATA_TEST_CASE(most_significant_active_bit, most_significant_active_bit_samples)
{
    BOOST_TEST(sample.n.most_significant_active_bit() == sample.expected);
}

static Integer_int_to_bool_sample_t get_bit_value_samples[] {
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  0, true},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  1, false},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 62, false},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 63, true},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  0,  false},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  63, false},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  64, true},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  65, false},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}), 126, false},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}), 127, true},
};

BOOST_DATA_TEST_CASE(get_bit_value, get_bit_value_samples)
{
    BOOST_TEST(sample.lhs.get_bit_value(sample.rhs) == sample.expected);
}

struct set_bit_value_sample_t {
    const Integer& n;
    const int bit;
    const bool v;
    const Integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const set_bit_value_sample_t& s) {
        os << "n: " << s.n << ", bit: " << s.bit << ", v: " << util::to_string(s.v)
           << ", expected: " << s.expected;
        return os;
    }
};

static set_bit_value_sample_t set_bit_value_samples[] {
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  0, false, WideInt<64>(0xa5a5'5a5a'ffff'0000)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  0, true,  WideInt<64>(0xa5a5'5a5a'ffff'0001)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  1, false, WideInt<64>(0xa5a5'5a5a'ffff'0001)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  1, true,  WideInt<64>(0xa5a5'5a5a'ffff'0003)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 62, true,  WideInt<64>(0xe5a5'5a5a'ffff'0001)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 63, false, WideInt<64>(0x25a5'5a5a'ffff'0001)},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  0,  false,
     WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  1,  true,
     WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0002})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  63, true,
     WideInt<128>({0xa000'0000'0000'0001, 0xd000'0000'0000'0000})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  64, false,
     WideInt<128>({0xa000'0000'0000'0000, 0x5000'0000'0000'0000})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  65, true,
     WideInt<128>({0xa000'0000'0000'0003, 0x5000'0000'0000'0000})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}), 126, true,
     WideInt<128>({0xe000'0000'0000'0001, 0x5000'0000'0000'0000})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}), 127, false,
     WideInt<128>({0x2000'0000'0000'0001, 0x5000'0000'0000'0000})},
};

BOOST_DATA_TEST_CASE(set_bit_value, set_bit_value_samples)
{
    Integer n(sample.n);
    BOOST_TEST(n.set_bit_value(sample.bit, sample.v) == sample.expected);
}

static binary_op_sample_t pow_samples[] {
    {WideInt<64>(3), WideInt<64>(0ul), WideInt<64>(1)},
    {WideInt<64>(3), WideInt<64>(1), WideInt<64>(3)},
    {WideInt<64>(3), WideInt<64>(2), WideInt<64>(9)},
    {WideInt<64>(3), WideInt<64>(3), WideInt<64>(27)},
    {WideInt<64>(3), WideInt<64>(4), WideInt<64>(81)},
    {WideInt<64>(10), WideInt<64>(5), WideInt<64>(100'000)},
    {WideInt<64>(0xff), WideInt<64>(5), WideInt<64>(1'078'203'909'375)},
};

BOOST_DATA_TEST_CASE(pow, pow_samples)
{
    Integer base = sample.lhs;
    const Integer& exponent = sample.rhs;
    const auto n = base.pow(exponent);
    BOOST_TEST(n == sample.expected);
}

static binary_mod_op_sample_t pow_mod_samples[] {
    {WideInt<64>(3), WideInt<64>(0), WideInt<64>(5), WideInt<64>(1)},
    {WideInt<64>(3), WideInt<64>(1), WideInt<64>(5), WideInt<64>(3)},
    {WideInt<64>(3), WideInt<64>(2), WideInt<64>(5), WideInt<64>(4)},
    {WideInt<64>(3), WideInt<64>(3), WideInt<64>(5), WideInt<64>(2)},
    {WideInt<64>(3), WideInt<64>(4), WideInt<64>(5), WideInt<64>(1)},
    {WideInt<64>(10), WideInt<64>(5), WideInt<64>(5), WideInt<64>(0)},
    {WideInt<64>(0xff), WideInt<64>(5), WideInt<64>(5), WideInt<64>(0)},
};

BOOST_DATA_TEST_CASE(pow_mod, pow_mod_samples)
{
    Integer base = sample.lhs;
    const Integer& exponent = sample.rhs;
    const Integer& mod = sample.mod;
    const auto n = base.pow_mod(exponent, mod);
    BOOST_TEST(n == sample.expected);
}

static int_to_Integer_sample_t pow2_samples[] {
    {0, WideInt<64>(1)},
    {1, WideInt<64>(2)},
    {15, WideInt<64>(0x8000)},
    {16, WideInt<64>(0x1'0000)},
    {63, WideInt<64>(0x8000'0000'0000'0000)},
    {64, WideInt<128>({1, 0})},
    {127, WideInt<128>({0x8000'0000'0000'0000, 0})},
    {128, WideInt<192>({1, 0, 0})},
    {255, WideInt<256>({0x8000'0000'0000'0000, 0, 0, 0})},
    {256, WideInt<320>({1, 0, 0, 0, 0})},
};

BOOST_DATA_TEST_CASE(pow2, pow2_samples)
{
    BOOST_TEST(Integer::pow2(sample.n) == sample.expected);
}

static Integer_to_bool_sample_t is_odd_samples[] {
    {WideInt<64>(0),  false},
    {WideInt<64>(1),  true},
    {WideInt<64>(2),  false},
    {WideInt<64>(3),  true},
    {WideInt<256>(0), false},
    {WideInt<256>(1), true},
};

BOOST_DATA_TEST_CASE(is_odd_even, is_odd_samples)
{
    BOOST_TEST(sample.n.is_odd() == sample.expected);
    BOOST_TEST(sample.n.is_even() != sample.expected);
}

static Integer_to_bool_sample_t is_zero_samples[] {
    {WideInt<64>(0),  true},
    {WideInt<64>(1),  false},
    {WideInt<64>(2),  false},
    {WideInt<256>(0), true},
    {WideInt<256>(1), false},
    {WideInt<256>(2), false},
};

BOOST_DATA_TEST_CASE(is_zero, is_zero_samples)
{
    BOOST_TEST(sample.n.is_zero() == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
