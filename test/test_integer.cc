#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <vector>
#include "integer.h"
#include "util.h"

using namespace grill;

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

BOOST_AUTO_TEST_CASE(get_bytes)
{
    wide_int<64> n;
    BOOST_TEST(n.get_num_blocks() == 1);
}

BOOST_AUTO_TEST_CASE(initial_value)
{
    wide_int<64> n;
    BOOST_TEST(n.ref_blocks()[0] == 0);
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
    BOOST_TEST(n1.is_blocks_owner() == false);
    BOOST_TEST(n2.is_blocks_owner() == true);
    BOOST_TEST(n1.ref_blocks() != n2.ref_blocks());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    wide_int<64> n1 = 123;
    const auto n2(std::move(n1));

    BOOST_TEST(n2.ref_blocks()[0] == 123);
    BOOST_TEST(n1.is_blocks_owner() == false);
    BOOST_TEST(n2.is_blocks_owner() == false);
    BOOST_TEST(n1.ref_blocks() == n2.ref_blocks());
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

BOOST_AUTO_TEST_CASE(add)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    n1 += n2;
    BOOST_TEST(n1.ref_blocks()[0] == 13);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(add_binary_operator)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    const auto n = n1 + n2;
    BOOST_TEST(n1.ref_blocks()[0] == 10);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
    BOOST_TEST(n.ref_blocks()[0] == 13);
    BOOST_TEST(n.is_blocks_owner() == true);
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
    BOOST_TEST(n.is_blocks_owner() == true);
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
    BOOST_TEST(n.is_blocks_owner() == true);
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
    BOOST_TEST(n.is_blocks_owner() == true);
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
    BOOST_TEST(n.is_blocks_owner() == true);
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
    BOOST_TEST(src.is_blocks_owner() == false);
    BOOST_TEST(n.is_blocks_owner() == false);
}

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

struct binary_op_sample_t {
    const integer& lhs;
    const integer& rhs;
    const integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const binary_op_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs << ", expected: " << s.expected;
        return os;
    }
};

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

struct integer_and_int_sample_t {
    const integer& lhs;
    int rhs;
    const integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const integer_and_int_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs << ", expected: " << s.expected;
        return os;
    }
};

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

static struct pow_sample_t {
    integer::block_t exponent;
    integer::block_t base;
    integer::block_t expected;

    friend std::ostream& operator<<(std::ostream& os, const pow_sample_t& s) {
        os << "exponent: " << s.exponent << ", base: " << s.base << ", expected: " << s.expected;
        return os;
    }
} pow_samples[] {
    {0, 3, 1},
    {1, 3, 3},
    {2, 3, 9},
    {3, 3, 27},
    {4, 3, 81},
    {5, 10, 10'0000},
    {5, 0xff, 1'078'203'909'375},
};

BOOST_DATA_TEST_CASE(pow, pow_samples)
{
    const wide_int<64> n1 = sample.base;
    const wide_int<64> e = sample.exponent;
    const auto n2 = n1.pow(e);
    BOOST_TEST(n2.ref_blocks()[0] == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
