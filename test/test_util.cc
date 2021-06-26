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
    {WideInt<64>(1),  WideInt<64>(1),  WideInt<64>(1)},
    {WideInt<64>(1),  WideInt<64>(2),  WideInt<64>(1)},

    {WideInt<64>(2),  WideInt<64>(1),  WideInt<64>(1)},
    {WideInt<64>(2),  WideInt<64>(2),  WideInt<64>(2)},
    {WideInt<64>(2),  WideInt<64>(3),  WideInt<64>(1)},
    {WideInt<64>(2),  WideInt<64>(4),  WideInt<64>(2)},

    {WideInt<64>(3),  WideInt<64>(1),  WideInt<64>(1)},
    {WideInt<64>(3),  WideInt<64>(2),  WideInt<64>(1)},
    {WideInt<64>(3),  WideInt<64>(3),  WideInt<64>(3)},
    {WideInt<64>(3),  WideInt<64>(4),  WideInt<64>(1)},
    {WideInt<64>(3),  WideInt<64>(5),  WideInt<64>(1)},
    {WideInt<64>(3),  WideInt<64>(6),  WideInt<64>(3)},

    {WideInt<64>(10),  WideInt<64>(15),  WideInt<64>(5)},
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
    {WideInt<64>(123), 123},
    {WideInt<64>(0x7fff'ffff'ffff'ffff), 0x7fff'ffff'ffff'ffff},
    {WideInt<64>(0x8000'0000'0000'0000), 0x8000'0000'0000'0000},
    {WideInt<64>(0xffff'ffff'ffff'ffff), 0xffff'ffff'ffff'ffff},
};

BOOST_DATA_TEST_CASE(to_uint, to_uint_samples)
{
    BOOST_TEST(util::to_uint(sample.n) == sample.expected);
}

static string_to_Integer_sample_t to_Integer_samples[] {
    {"0", constant::Zero},
    {"1", constant::One},
    {"123", WideInt<64>(123)},
    {"9223372036854775807", WideInt<64>(0x7fff'ffff'ffff'ffff)},
    {"9223372036854775808", WideInt<64>(0x8000'0000'0000'0000)},
    {"18446744073709551615", WideInt<64>(0xffff'ffff'ffff'ffff)},

    {"0x0", constant::Zero},
    {"0x1", constant::One},
    {"0x123", WideInt<64>(0x123)},
    {"0xA5", WideInt<64>(0xa5)},
    {"0xffffffffffffffff", WideInt<64>(0xffff'ffff'ffff'ffff)},
    {"0x1ffffeeeeddddcccc", WideInt<128>({1, 0xffff'eeee'dddd'cccc})},
    {"0x123456789abcdef0ffffeeeeddddcccc",
     WideInt<128>({0x1234'5678'9abc'def0, 0xffff'eeee'dddd'cccc})},
    {"0x1230000111122223333123456789abcdef0ffffeeeeddddcccc",
     WideInt<256>({0x123, 0x1111'2222'3333, 0x1234'5678'9abc'def0, 0xffff'eeee'dddd'cccc})},
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
