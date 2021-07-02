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
    {Integer({0x0123456789abcdef}), "0123456789abcdef"},
    {Integer({0xfedcba9876543210}), "fedcba9876543210"},
    {Integer({0x0123456789abcdef, 0x11223344aabbccdd}), "0123456789abcdef" "11223344aabbccdd"},
};

BOOST_DATA_TEST_CASE(cast_string, cast_string_samples)
{
    BOOST_TEST(static_cast<std::string>(sample.n) == sample.expected);
}

BOOST_AUTO_TEST_CASE(substitution)
{
    Integer n({1, 2, 3, 4});
    n = {5};

    const Integer::block_t expected[] = {5};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(move_substitution)
{
    Integer src({1, 2, 3, 4});
    const Integer n = std::move(src);

    const Integer::block_t expected[] = {4, 3, 2, 1};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

static cmp_sample_t equal_samples[] {
    {Integer({7}), Integer({7}), true},
    {Integer({7}), Integer({9}), false},
    {Integer({7}), Integer({5}), false},
    {Integer({7}), Integer({7}), true},
    {Integer({7}), Integer({9}), false},
    {Integer({7}), Integer({5}), false},
    {Integer({7}), Integer({7}), true},
    {Integer({7}), Integer({9}), false},
    {Integer({7}), Integer({5}), false},
};

BOOST_DATA_TEST_CASE(equal, equal_samples)
{
    BOOST_TEST((sample.lhs == sample.rhs) == sample.expected);
}

static cmp_sample_t not_equal_samples[] {
    {Integer({7}), Integer({7}), false},
    {Integer({7}), Integer({9}), true},
    {Integer({7}), Integer({5}), true},
    {Integer({7}), Integer({7}), false},
    {Integer({7}), Integer({9}), true},
    {Integer({7}), Integer({5}), true},
    {Integer({7}), Integer({7}), false},
    {Integer({7}), Integer({9}), true},
    {Integer({7}), Integer({5}), true},
};

BOOST_DATA_TEST_CASE(not_equal, not_equal_samples)
{
    BOOST_TEST((sample.lhs != sample.rhs) == sample.expected);
}

static cmp_sample_t gt_eq_samples[] {
    {Integer({7}), Integer({7}), true},
    {Integer({7}), Integer({9}), false},
    {Integer({7}), Integer({5}), true},
    {Integer({7}), Integer({7}), true},
    {Integer({7}), Integer({9}), false},
    {Integer({7}), Integer({5}), true},
    {Integer({7}), Integer({7}), true},
    {Integer({7}), Integer({9}), false},
    {Integer({7}), Integer({5}), true},
};

BOOST_DATA_TEST_CASE(gt_eq, gt_eq_samples)
{
    BOOST_TEST((sample.lhs >= sample.rhs) == sample.expected);
}

static cmp_sample_t lt_eq_samples[] {
    {Integer({7}), Integer({7}), true},
    {Integer({7}), Integer({9}), true},
    {Integer({7}), Integer({5}), false},
    {Integer({7}), Integer({7}), true},
    {Integer({7}), Integer({9}), true},
    {Integer({7}), Integer({5}), false},
    {Integer({7}), Integer({7}), true},
    {Integer({7}), Integer({9}), true},
    {Integer({7}), Integer({5}), false},
};

BOOST_DATA_TEST_CASE(lt_eq, lt_eq_samples)
{
    BOOST_TEST((sample.lhs <= sample.rhs) == sample.expected);
}

static binary_op_sample_t bitwise_and_samples[] {
    {Integer({0xea}), Integer({0xcc}), Integer({0xc8})},
    {Integer({0xea}), Integer({0xcc}), Integer({0xc8})},
    {Integer({0xea}), Integer({1,2,3,0xcc}), Integer({0xc8})},
};

BOOST_DATA_TEST_CASE(bitwise_and, bitwise_and_samples)
{
    const Integer result = sample.lhs & sample.rhs;
    BOOST_TEST(result == sample.expected);
    BOOST_TEST(result.get_num_blocks() == sample.expected.get_num_blocks());
}

static Integer_and_int_sample_t left_shift_samples[] {
    {Integer({0x40002000100080a5}), 0, Integer({0x40002000100080a5})},
    {Integer({0x40002000100080a5}), 1, Integer({0x800040002001014a})},
    // TODO: remove leading 0s
    {Integer({0, 0x40002000100080a5}), 8, Integer({0x40, 0x002000100080a500})},
    {Integer({0, 0x40002000100080a5}), 64, Integer({0x40002000100080a5, 0})},
    {Integer({0, 0x40002000100080a5}), 127, Integer({0x8000000000000000, 0})},
    {Integer({0, 0x40002000100080a5}), 128, Integer({0, 0})},
    {Integer({0, 0x40002000100080a5}), 129, Integer({0, 0})},
    {Integer({0, 0x40002000100080a5}), 256, Integer({0, 0})},
};

BOOST_DATA_TEST_CASE(left_shift, left_shift_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n <<= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

static Integer_and_int_sample_t right_shift_samples[] {
    {Integer({0x40002000100080a5}), 0, Integer({0x40002000100080a5})},
    {Integer({0x800040002001014a}), 1, Integer({0x40002000100080a5})},
    {Integer({0x40, 0x002000100080a500}), 8, Integer({0x40002000100080a5})},
    {Integer({0x40002000100080a5, 0}),   64, Integer({0x40002000100080a5})},
    {Integer({0x8fff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 127, Integer({1})},
    {Integer({0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 128, Integer({0})},
    {Integer({0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 129, Integer({0})},
    {Integer({0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 256, Integer({0})},
};

BOOST_DATA_TEST_CASE(right_shift, right_shift_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n >>= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

static unary_op_sample_t prefix_increment_samples[] {
    {Integer({0}), Integer({1})},
    {Integer({1}), Integer({2})},
    {Integer({0xffff'ffff'ffff'ffff}), Integer({1, 0})},
};

BOOST_DATA_TEST_CASE(prefix_increment, prefix_increment_samples)
{
    Integer n = sample.n;
    BOOST_TEST(++n == sample.expected);
}

static Integer_to_int_sample_t most_significant_active_bit_samples[] {
    {Integer({0}), 0},
    {Integer({1}), 1},
    {Integer({2}), 2},
    {Integer({3}), 2},
    {Integer({0xa5}), 8},
    {Integer({0x4123'4567'89ab'cdef}), 63},
    {Integer({0x8123'4567'89ab'cdef}), 64},
    {Integer({0, 0x8123'4567'89ab'cdef}), 64},
    {Integer({1, 0x8123'4567'89ab'cdef}), 65},
    {Integer({0xa0, 0x8123'4567'89ab'cdef}), 72},
};

BOOST_DATA_TEST_CASE(most_significant_active_bit, most_significant_active_bit_samples)
{
    BOOST_TEST(sample.n.most_significant_active_bit() == sample.expected);
}

static binary_op_sample_t pow_samples[] {
    {Integer({3}), Integer({0ul}), Integer({1})},
    {Integer({3}), Integer({1}), Integer({3})},
    {Integer({3}), Integer({2}), Integer({9})},
    {Integer({3}), Integer({3}), Integer({27})},
    {Integer({3}), Integer({4}), Integer({81})},
    {Integer({10}), Integer({5}), Integer({100'000})},
    {Integer({0xff}), Integer({5}), Integer({1'078'203'909'375})},
};

BOOST_DATA_TEST_CASE(pow, pow_samples)
{
    Integer base = sample.lhs;
    const Integer& exponent = sample.rhs;
    const auto n = base.pow(exponent);
    BOOST_TEST(n == sample.expected);
}

static binary_mod_op_sample_t pow_mod_samples[] {
    {Integer({3}), Integer({0}), Integer({5}), Integer({1})},
    {Integer({3}), Integer({1}), Integer({5}), Integer({3})},
    {Integer({3}), Integer({2}), Integer({5}), Integer({4})},
    {Integer({3}), Integer({3}), Integer({5}), Integer({2})},
    {Integer({3}), Integer({4}), Integer({5}), Integer({1})},
    {Integer({10}), Integer({5}), Integer({5}), Integer({0})},
    {Integer({0xff}), Integer({5}), Integer({5}), Integer({0})},
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
    {0, Integer({1})},
    {1, Integer({2})},
    {15, Integer({0x8000})},
    {16, Integer({0x1'0000})},
    {63, Integer({0x8000'0000'0000'0000})},
    {64, Integer({1, 0})},
    {127, Integer({0x8000'0000'0000'0000, 0})},
    {128, Integer({1, 0, 0})},
    {255, Integer({0x8000'0000'0000'0000, 0, 0, 0})},
    {256, Integer({1, 0, 0, 0, 0})},
};

BOOST_DATA_TEST_CASE(pow2, pow2_samples)
{
    BOOST_TEST(Integer::pow2(sample.n) == sample.expected);
}

static Integer_to_bool_sample_t is_odd_samples[] {
    {Integer({0}), false},
    {Integer({1}), true},
    {Integer({2}), false},
    {Integer({3}), true},
    {Integer({0}), false},
    {Integer({1}), true},
};

BOOST_DATA_TEST_CASE(is_odd_even, is_odd_samples)
{
    BOOST_TEST(sample.n.is_odd() == sample.expected);
    BOOST_TEST(sample.n.is_even() != sample.expected);
}

static Integer_to_bool_sample_t is_zero_samples[] {
    {Integer({0}), true},
    {Integer({1}), false},
    {Integer({2}), false},
    {Integer({0}), true},
    {Integer({1}), false},
    {Integer({2}), false},
};

BOOST_DATA_TEST_CASE(is_zero, is_zero_samples)
{
    BOOST_TEST(sample.n.is_zero() == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
