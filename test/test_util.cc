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

BOOST_AUTO_TEST_SUITE_END()
