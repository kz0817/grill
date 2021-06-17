#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "util.h"
#include "sample_types.h"
#include "constant.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_util)

struct gcd_sample_t {
    const integer& a;
    const integer& b;
    const integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const gcd_sample_t& s) {
        os << "a: " << s.a << ", b: " << s.b << ", expected: " << s.expected;
        return os;
    }
};

static gcd_sample_t gcd_samples[] {
    {wide_int<64>(1),  wide_int<64>(1),  wide_int<64>(1)},
    {wide_int<64>(1),  wide_int<64>(2),  wide_int<64>(1)},

    {wide_int<64>(2),  wide_int<64>(1),  wide_int<64>(1)},
    {wide_int<64>(2),  wide_int<64>(2),  wide_int<64>(2)},
    {wide_int<64>(2),  wide_int<64>(3),  wide_int<64>(1)},
    {wide_int<64>(2),  wide_int<64>(4),  wide_int<64>(2)},

    {wide_int<64>(3),  wide_int<64>(1),  wide_int<64>(1)},
    {wide_int<64>(3),  wide_int<64>(2),  wide_int<64>(1)},
    {wide_int<64>(3),  wide_int<64>(3),  wide_int<64>(3)},
    {wide_int<64>(3),  wide_int<64>(4),  wide_int<64>(1)},
    {wide_int<64>(3),  wide_int<64>(5),  wide_int<64>(1)},
    {wide_int<64>(3),  wide_int<64>(6),  wide_int<64>(3)},

    {wide_int<64>(10),  wide_int<64>(15),  wide_int<64>(5)},
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

static integer_to_uint_sample_t to_uint_samples[] {
    {constant::Zero, 0},
    {constant::One,  1},
    {wide_int<64>(123), 123},
    {wide_int<64>(0x7fff'ffff'ffff'ffff), 0x7fff'ffff'ffff'ffff},
    {wide_int<64>(0x8000'0000'0000'0000), 0x8000'0000'0000'0000},
    {wide_int<64>(0xffff'ffff'ffff'ffff), 0xffff'ffff'ffff'ffff},
};

BOOST_DATA_TEST_CASE(to_uint, to_uint_samples)
{
    BOOST_TEST(util::to_uint(sample.n) == sample.expected);
}

static string_to_integer_sample_t to_integer_samples[] {
    {"0", constant::Zero},
    {"1", constant::One},
    {"123", wide_int<64>(123)},
    {"9223372036854775807", wide_int<64>(0x7fff'ffff'ffff'ffff)},
    {"9223372036854775808", wide_int<64>(0x8000'0000'0000'0000)},
    {"18446744073709551615", wide_int<64>(0xffff'ffff'ffff'ffff)},
};

BOOST_DATA_TEST_CASE(to_integer, to_integer_samples)
{
    BOOST_TEST(util::to_integer(sample.s) == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
