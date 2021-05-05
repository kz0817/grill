#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "util.h"

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

BOOST_AUTO_TEST_SUITE_END()
