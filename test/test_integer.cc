#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <vector>
#include "integer.h"
#include "constant.h"
#include "util.h"

using namespace grill;

const integer::block_t MaxBlockValue = 0xffff'ffff'ffff'ffff;

struct cmp_sample_t {
    const integer& lhs;
    const integer& rhs;
    bool expected;

    friend std::ostream& operator<<(std::ostream& os, const cmp_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs <<
              ", expected: " << util::to_string(s.expected);
        return os;
    }
};

struct unary_op_sample_t {
    const integer& n;
    const integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const unary_op_sample_t& s) {
        os << "lhs: " << ", expected: " << s.expected;
        return os;
    }
};

struct binary_op_sample_t {
    const integer& lhs;
    const integer& rhs;
    const integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const binary_op_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs << ", expected: " << s.expected;
        return os;
    }
};

struct integer_and_int_sample_t {
    const integer& lhs;
    int rhs;
    const integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const integer_and_int_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs << ", expected: " << s.expected;
        return os;
    }
};

struct integer_to_int_sample_t {
    const integer& n;
    const int expected;

    friend std::ostream& operator<<(std::ostream& os, const integer_to_int_sample_t& s) {
        os << "n: " << s.n << ", expected: " << s.expected;
        return os;
    }
};

struct integer_int_to_bool_sample_t {
    const integer& lhs;
    const int rhs;
    const bool expected;

    friend std::ostream& operator<<(std::ostream& os, const integer_int_to_bool_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs
           << ", expected: " << util::to_string(s.expected);
        return os;
    }
};

struct integer_to_bool_sample_t {
    const integer& n;
    const bool expected;

    friend std::ostream& operator<<(std::ostream& os, const integer_to_bool_sample_t& s) {
        os << "n: " << s.n << ", expected: " << util::to_string(s.expected);
        return os;
    }
};

static std::vector<integer::block_t> create_block_vector(const integer& n) {
    const int num_blocks = n.get_num_blocks();
    std::vector<integer::block_t> vec;
    vec.reserve(num_blocks);
    const integer::block_t* blocks = n.ref_blocks();
    for (int i = 0; i < num_blocks; i++)
        vec.emplace_back(blocks[i]);
    return vec;
}

BOOST_AUTO_TEST_SUITE(test_suite_integer)

BOOST_AUTO_TEST_CASE(default_constructor)
{
    wide_int<64> n;
    BOOST_TEST(n.ref_blocks()[0] == 0);
    BOOST_TEST(n.get_num_blocks() == 1);
}

BOOST_AUTO_TEST_CASE(one_initial_value)
{
    wide_int<64> n(5);
    BOOST_TEST(n.ref_blocks()[0] == 5);
}

BOOST_AUTO_TEST_CASE(given_initial_value)
{
    const uint64_t init_value[] = {123};
    wide_int<64> n(init_value);
    BOOST_TEST(n.ref_blocks()[0] == 123);
}

BOOST_AUTO_TEST_CASE(given_initial_value_many_blocks)
{
    const uint64_t init_value[] = {1, 2, 3, 4};
    wide_int<256> n(init_value);
    const integer::block_t expected[] = {4, 3, 2, 1};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(given_initial_value_with_initializer_list)
{
    wide_int<64> n({123});
    BOOST_TEST(n.ref_blocks()[0] == 123);
}

BOOST_AUTO_TEST_CASE(given_initial_value_with_initializer_list_with_mutiple_data)
{
    wide_int<256> n({1, 2, 3});
    const integer::block_t expected[] = {3, 2, 1, 0};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(given_initial_value_with_initializer_list_with_full_data)
{
    wide_int<256> n({1, 2, 3, 4});
    const integer::block_t expected[] = {4, 3, 2, 1};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    wide_int<64> n1 = 123;
    const auto n2(n1);

    BOOST_TEST(n2.ref_blocks()[0] == 123);
    BOOST_TEST(n1.ref_blocks() != n2.ref_blocks());
    BOOST_TEST(n1.ref_blocks() != nullptr);
    BOOST_TEST(n2.ref_blocks() != nullptr);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    wide_int<64> n1 = 123;
    const auto n1_ref_blocks = n1.ref_blocks();
    const auto n2(std::move(n1));

    BOOST_TEST(n2.ref_blocks()[0] == 123);
    BOOST_TEST(n1_ref_blocks == n2.ref_blocks());
    BOOST_TEST(n1.ref_blocks() == nullptr);
}

static struct cast_string_sample_t {
    const integer& n;
    std::string expected;

    friend std::ostream& operator<<(std::ostream& os, const cast_string_sample_t& s) {
        os << "n.num_blocks: " << s.n.get_num_blocks() << ", expected: " << s.expected;
        return os;
    }
} cast_string_samples[] {
    {wide_int<64>(0x0123456789abcdef), "0123456789abcdef"},
    {wide_int<64>(0xfedcba9876543210), "fedcba9876543210"},
    {wide_int<128>(1), "0000000000000000" "0000000000000001"},
    {wide_int<128>({0x0123456789abcdef, 0x11223344aabbccdd}),
                   "0123456789abcdef" "11223344aabbccdd"},
};

BOOST_DATA_TEST_CASE(cast_string, cast_string_samples)
{
    BOOST_TEST(static_cast<std::string>(sample.n) == sample.expected);
}

//
// += operator
//
static binary_op_sample_t add_subst_operator_samples[] = {
    {
        wide_int<64>(10),
        wide_int<64>(3),
        wide_int<64>(13)
    }, {
        wide_int<64>(MaxBlockValue),
        wide_int<64>(1),
        constant::Zero
    },

    {
        wide_int<256>({1, 2, 3, 4}),
        wide_int<128>({10, 20}),
        wide_int<256>({1, 2, 13, 24})
    }, {
        wide_int<256>({1, MaxBlockValue, MaxBlockValue, MaxBlockValue}),
        wide_int<128>({1, 2}),
        wide_int<256>({2, 0, 1, 1})
    },

    {
        wide_int<128>({1, 2}),
        wide_int<256>({10, 20, 30, 40}),
        wide_int<128>({31, 42})
    }, {
        wide_int<128>({1, MaxBlockValue}),
        wide_int<256>({1, 2, 3, 4}),
        wide_int<128>({5, 3})
    },
};

BOOST_DATA_TEST_CASE(add_subst_operator, add_subst_operator_samples)
{
    integer lhs = sample.lhs;
    lhs += sample.rhs;
    BOOST_TEST(lhs == sample.expected);
}

BOOST_AUTO_TEST_CASE(add_binary_operator)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    const auto n = n1 + n2;
    BOOST_TEST(n1.ref_blocks()[0] == 10);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
    BOOST_TEST(n.ref_blocks()[0] == 13);
}

BOOST_AUTO_TEST_CASE(sub)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    n1 -= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 7);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(sub_binary_opreator)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    const auto n = n1 - n2;
    BOOST_TEST(n1.ref_blocks()[0] == 10);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
    BOOST_TEST(n.ref_blocks()[0] == 7);
}

BOOST_AUTO_TEST_CASE(mul)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    n1 *= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 30);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(mul_binary_opreator)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    const auto n = n1 * n2;
    BOOST_TEST(n1.ref_blocks()[0] == 10);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
    BOOST_TEST(n.ref_blocks()[0] == 30);
}

BOOST_AUTO_TEST_CASE(div)
{
    wide_int<64> n1 = 11;
    wide_int<64> n2 = 4;

    n1 /= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 2);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
}

BOOST_AUTO_TEST_CASE(div_binary_opreator)
{
    wide_int<64> n1 = 11;
    wide_int<64> n2 = 4;

    const auto n = n1 / n2;
    BOOST_TEST(n1.ref_blocks()[0] == 11);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
    BOOST_TEST(n.ref_blocks()[0] == 2);
}

BOOST_AUTO_TEST_CASE(mod)
{
    wide_int<64> n1 = 11;
    wide_int<64> n2 = 4;

    n1 %= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 3);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
}

BOOST_AUTO_TEST_CASE(mod_binary_opreator)
{
    wide_int<64> n1 = 11;
    wide_int<64> n2 = 4;

    const auto n = n1 % n2;
    BOOST_TEST(n1.ref_blocks()[0] == 11);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
    BOOST_TEST(n.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(substitution)
{
    wide_int<256> n({1, 2, 3, 4});
    n = 5;

    const integer::block_t expected[] = {5, 0, 0, 0};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(move_substitution)
{
    wide_int<256> src({1, 2, 3, 4});
    const integer n = std::move(src);

    const integer::block_t expected[] = {4, 3, 2, 1};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

static cmp_sample_t equal_samples[] {
    {wide_int<64>(7),  wide_int<64>(7),  true},
    {wide_int<64>(7),  wide_int<64>(9),  false},
    {wide_int<64>(7),  wide_int<64>(5),  false},
    {wide_int<256>(7), wide_int<64>(7),  true},
    {wide_int<256>(7), wide_int<64>(9),  false},
    {wide_int<256>(7), wide_int<64>(5),  false},
    {wide_int<64>(7),  wide_int<256>(7), true},
    {wide_int<64>(7),  wide_int<256>(9), false},
    {wide_int<64>(7),  wide_int<256>(5), false},
};

BOOST_DATA_TEST_CASE(equal, equal_samples)
{
    BOOST_TEST((sample.lhs == sample.rhs) == sample.expected);
}

static cmp_sample_t not_equal_samples[] {
    {wide_int<64>(7),  wide_int<64>(7),  false},
    {wide_int<64>(7),  wide_int<64>(9),  true},
    {wide_int<64>(7),  wide_int<64>(5),  true},
    {wide_int<256>(7), wide_int<64>(7),  false},
    {wide_int<256>(7), wide_int<64>(9),  true},
    {wide_int<256>(7), wide_int<64>(5),  true},
    {wide_int<64>(7),  wide_int<256>(7), false},
    {wide_int<64>(7),  wide_int<256>(9), true},
    {wide_int<64>(7),  wide_int<256>(5), true},
};

BOOST_DATA_TEST_CASE(not_equal, not_equal_samples)
{
    BOOST_TEST((sample.lhs != sample.rhs) == sample.expected);
}

static cmp_sample_t gt_eq_samples[] {
    {wide_int<64>(7),  wide_int<64>(7),  true},
    {wide_int<64>(7),  wide_int<64>(9),  false},
    {wide_int<64>(7),  wide_int<64>(5),  true},
    {wide_int<256>(7), wide_int<64>(7),  true},
    {wide_int<256>(7), wide_int<64>(9),  false},
    {wide_int<256>(7), wide_int<64>(5),  true},
    {wide_int<64>(7),  wide_int<256>(7), true},
    {wide_int<64>(7),  wide_int<256>(9), false},
    {wide_int<64>(7),  wide_int<256>(5), true},
};

BOOST_DATA_TEST_CASE(gt_eq, gt_eq_samples)
{
    BOOST_TEST((sample.lhs >= sample.rhs) == sample.expected);
}

static cmp_sample_t lt_eq_samples[] {
    {wide_int<64>(7),  wide_int<64>(7),  true},
    {wide_int<64>(7),  wide_int<64>(9),  true},
    {wide_int<64>(7),  wide_int<64>(5),  false},
    {wide_int<256>(7), wide_int<64>(7),  true},
    {wide_int<256>(7), wide_int<64>(9),  true},
    {wide_int<256>(7), wide_int<64>(5),  false},
    {wide_int<64>(7),  wide_int<256>(7), true},
    {wide_int<64>(7),  wide_int<256>(9), true},
    {wide_int<64>(7),  wide_int<256>(5), false},
};

BOOST_DATA_TEST_CASE(lt_eq, lt_eq_samples)
{
    BOOST_TEST((sample.lhs <= sample.rhs) == sample.expected);
}

static binary_op_sample_t bitwise_and_samples[] {
    {wide_int<64>(0xea),  wide_int<64>(0xcc),  wide_int<64>(0xc8)},
    {wide_int<256>(0xea), wide_int<64>(0xcc),  wide_int<256>(0xc8)},
    {wide_int<64>(0xea),  wide_int<256>(0xcc), wide_int<256>(0xc8)},
};

BOOST_DATA_TEST_CASE(bitwise_and, bitwise_and_samples)
{
    const integer result = sample.lhs & sample.rhs;
    BOOST_TEST(result == sample.expected);
    BOOST_TEST(result.get_num_blocks() == sample.expected.get_num_blocks());
}

static integer_and_int_sample_t left_shift_samples[] {
    {wide_int<64>(0x40002000100080a5), 0, wide_int<64>(0x40002000100080a5)},
    {wide_int<64>(0x40002000100080a5), 1, wide_int<64>(0x800040002001014a)},
    {wide_int<128>(0x40002000100080a5), 8, wide_int<128>({0x40, 0x002000100080a500})},
    {wide_int<128>(0x40002000100080a5), 64, wide_int<128>({0x40002000100080a5, 0})},
    {wide_int<128>(0x40002000100080a5), 127, wide_int<128>({0x8000000000000000, 0})},
    {wide_int<128>(0x40002000100080a5), 128, wide_int<128>({0, 0})},
    {wide_int<128>(0x40002000100080a5), 129, wide_int<128>({0, 0})},
    {wide_int<128>(0x40002000100080a5), 256, wide_int<128>({0, 0})},
};

BOOST_DATA_TEST_CASE(left_shift, left_shift_samples)
{
    integer n = sample.lhs;
    BOOST_TEST((n <<= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

static integer_and_int_sample_t right_shift_samples[] {
    {wide_int<64>(0x40002000100080a5), 0, wide_int<64>(0x40002000100080a5)},
    {wide_int<64>(0x800040002001014a), 1, wide_int<64>(0x40002000100080a5)},
    {wide_int<128>({0x40, 0x002000100080a500}), 8, wide_int<128>(0x40002000100080a5)},
    {wide_int<128>({0x40002000100080a5, 0}),   64, wide_int<128>(0x40002000100080a5)},
    {wide_int<128>({0x8fff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 127, wide_int<128>(1)},
    {wide_int<128>({0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 128, wide_int<128>()},
    {wide_int<128>({0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 129, wide_int<128>()},
    {wide_int<128>({0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), 256, wide_int<128>()},
};

BOOST_DATA_TEST_CASE(right_shift, right_shift_samples)
{
    integer n = sample.lhs;
    BOOST_TEST((n >>= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

static unary_op_sample_t prefix_increment_samples[] {
    {wide_int<64>(),  wide_int<64>(1)},
    {wide_int<64>(1), wide_int<64>(2)},
    {wide_int<64>(0xffff'ffff'ffff'ffff), wide_int<64>()},
};

BOOST_DATA_TEST_CASE(prefix_increment, prefix_increment_samples)
{
    integer n = sample.n;
    BOOST_TEST(++n == sample.expected);
}

static integer_to_int_sample_t most_significant_active_bit_samples[] {
    {wide_int<64>(),  0},
    {wide_int<64>(1), 1},
    {wide_int<64>(2), 2},
    {wide_int<64>(3), 2},
    {wide_int<64>(0xa5), 8},
    {wide_int<64>(0x4123'4567'89ab'cdef), 63},
    {wide_int<64>(0x8123'4567'89ab'cdef), 64},
    {wide_int<128>({0, 0x8123'4567'89ab'cdef}), 64},
    {wide_int<128>({1, 0x8123'4567'89ab'cdef}), 65},
    {wide_int<128>({0xa0, 0x8123'4567'89ab'cdef}), 72},
};

BOOST_DATA_TEST_CASE(most_significant_active_bit, most_significant_active_bit_samples)
{
    BOOST_TEST(sample.n.most_significant_active_bit() == sample.expected);
}

static integer_int_to_bool_sample_t get_bit_value_samples[] {
    {wide_int<64>(0xa5a5'5a5a'ffff'0001),  0, true},
    {wide_int<64>(0xa5a5'5a5a'ffff'0001),  1, false},
    {wide_int<64>(0xa5a5'5a5a'ffff'0001), 62, false},
    {wide_int<64>(0xa5a5'5a5a'ffff'0001), 63, true},
    {wide_int<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  0,  false},
    {wide_int<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  63, false},
    {wide_int<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  64, true},
    {wide_int<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  65, false},
    {wide_int<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}), 126, false},
    {wide_int<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}), 127, true},
};

BOOST_DATA_TEST_CASE(get_bit_value, get_bit_value_samples)
{
    BOOST_TEST(sample.lhs.get_bit_value(sample.rhs) == sample.expected);
}

static binary_op_sample_t pow_samples[] {
    {wide_int<64>(3), wide_int<64>(0ul), wide_int<64>(1)},
    {wide_int<64>(3), wide_int<64>(1), wide_int<64>(3)},
    {wide_int<64>(3), wide_int<64>(2), wide_int<64>(9)},
    {wide_int<64>(3), wide_int<64>(3), wide_int<64>(27)},
    {wide_int<64>(3), wide_int<64>(4), wide_int<64>(81)},
    {wide_int<64>(10), wide_int<64>(5), wide_int<64>(100'000)},
    {wide_int<64>(0xff), wide_int<64>(5), wide_int<64>(1'078'203'909'375)},
};

BOOST_DATA_TEST_CASE(pow, pow_samples)
{
    integer base = sample.lhs;
    const integer& exponent = sample.rhs;
    const auto n = base.pow(exponent);
    BOOST_TEST(n == sample.expected);
}

static integer_to_bool_sample_t is_odd_samples[] {
    {wide_int<64>(),   false},
    {wide_int<64>(1),  true},
    {wide_int<64>(2),  false},
    {wide_int<64>(3),  true},
    {wide_int<256>(),  false},
    {wide_int<256>(1), true},
};

BOOST_DATA_TEST_CASE(is_odd_even, is_odd_samples)
{
    BOOST_TEST(sample.n.is_odd() == sample.expected);
    BOOST_TEST(sample.n.is_even() != sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
