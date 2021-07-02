#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "util.h"
#include "sample_types.h"
#include "constant.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_util)

struct gcd_sample_t {
    const Integer& a;
    const Integer& b;
    const Integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const gcd_sample_t& s) {
        os << "a: " << s.a << ", b: " << s.b << ", expected: " << s.expected;
        return os;
    }
};

static gcd_sample_t gcd_samples[] {
    {Integer({1}),  Integer({1}),  Integer({1})},
    {Integer({1}),  Integer({2}),  Integer({1})},

    {Integer({2}),  Integer({1}),  Integer({1})},
    {Integer({2}),  Integer({2}),  Integer({2})},
    {Integer({2}),  Integer({3}),  Integer({1})},
    {Integer({2}),  Integer({4}),  Integer({2})},

    {Integer({3}),  Integer({1}),  Integer({1})},
    {Integer({3}),  Integer({2}),  Integer({1})},
    {Integer({3}),  Integer({3}),  Integer({3})},
    {Integer({3}),  Integer({4}),  Integer({1})},
    {Integer({3}),  Integer({5}),  Integer({1})},
    {Integer({3}),  Integer({6}),  Integer({3})},

    {Integer({10}),  Integer({15}),  Integer({5})},
};

BOOST_DATA_TEST_CASE(equal, gcd_samples)
{
    BOOST_TEST(util::gcd(sample.a, sample.b) == sample.expected);
}

BOOST_AUTO_TEST_CASE(to_string_of_true)
{
    BOOST_TEST(util::to_string(true) == "true");
}

BOOST_AUTO_TEST_CASE(to_string_of_false)
{
    BOOST_TEST(util::to_string(false) == "false");
}

static Integer_to_uint_sample_t to_uint_samples[] {
    {constant::Zero, 0},
    {constant::One,  1},
    {Integer({123}), 123},
    {Integer({0x7fff'ffff'ffff'ffff}), 0x7fff'ffff'ffff'ffff},
    {Integer({0x8000'0000'0000'0000}), 0x8000'0000'0000'0000},
    {Integer({0xffff'ffff'ffff'ffff}), 0xffff'ffff'ffff'ffff},
};

BOOST_DATA_TEST_CASE(to_uint, to_uint_samples)
{
    BOOST_TEST(util::to_uint(sample.n) == sample.expected);
}

static string_to_Integer_sample_t to_Integer_samples[] {
    {"0", constant::Zero},
    {"1", constant::One},
    {"123", Integer({123})},
    {"9223372036854775807", Integer({0x7fff'ffff'ffff'ffff})},
    {"9223372036854775808", Integer({0x8000'0000'0000'0000})},
    {"18446744073709551615", Integer({0xffff'ffff'ffff'ffff})},

    {"0x0", constant::Zero},
    {"0x1", constant::One},
    {"0x123", Integer({0x123})},
    {"0xA5", Integer({0xa5})},
    {"0xffffffffffffffff", Integer({0xffff'ffff'ffff'ffff})},
    {"0x1ffffeeeeddddcccc", Integer({1, 0xffff'eeee'dddd'cccc})},
    {"0x123456789abcdef0ffffeeeeddddcccc",
     Integer({0x1234'5678'9abc'def0, 0xffff'eeee'dddd'cccc})},
    {"0x1230000111122223333123456789abcdef0ffffeeeeddddcccc",
     Integer({0x123, 0x1111'2222'3333, 0x1234'5678'9abc'def0, 0xffff'eeee'dddd'cccc})},
};

BOOST_DATA_TEST_CASE(to_Integer, to_Integer_samples)
{
    BOOST_TEST(util::to_Integer(sample.s) == sample.expected);
}

struct get_random_sample_t {
    const std::size_t bit_length;
    const std::size_t expected_num_blocks;
    const std::size_t most_significant_bit_boundary;

    friend std::ostream& operator<<(std::ostream& os, const get_random_sample_t& s) {
        os << "bit_length: " << s.bit_length <<
            ", expected_num_blocks: " << s.expected_num_blocks <<
            ", most_significant_bit_boundary: " << s.most_significant_bit_boundary;
        return os;
    }
};

static get_random_sample_t get_random_samples[] {
    {3,   1, 3},
    {63,  1, 63},
    {64,  1, 64},
    {65,  2, 65},
    {128, 2, 128},
    {129, 3, 129},
};

BOOST_DATA_TEST_CASE(get_random, get_random_samples)
{
    const Integer n = util::get_random(sample.bit_length);
    BOOST_TEST(n.get_num_blocks() == sample.expected_num_blocks);
    BOOST_TEST(n.most_significant_active_bit() <= sample.most_significant_bit_boundary);
}

BOOST_AUTO_TEST_CASE(get_random_prime)
{
    const Integer n = util::get_random_prime(32);
    BOOST_TEST(n.get_num_blocks() == 1);
    BOOST_TEST(n.most_significant_active_bit() <= 32);
}

BOOST_AUTO_TEST_SUITE_END()
