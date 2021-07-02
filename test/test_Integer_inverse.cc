#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "sample_types.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_Integer_inverse)

static binary_op_sample_t inverse_samples[] = {
    {Integer({1}), Integer({5}), Integer({1})},
    {Integer({2}), Integer({5}), Integer({3})},
    {Integer({3}), Integer({5}), Integer({2})},
    {Integer({4}), Integer({5}), Integer({4})},

    {Integer({6}), Integer({5}), Integer({1})},
    {Integer({7}), Integer({5}), Integer({3})},
    {Integer({8}), Integer({5}), Integer({2})},
    {Integer({9}), Integer({5}), Integer({4})},

    // TODO: Add samples with greater digits.
    {Integer({1}), Integer({0x1a770ecab58cfb0d}), Integer({1})},
    {Integer({3}), Integer({0x1a770ecab58cfb0d}), Integer({0x11a4b48723b35209})},
    {Integer({3}), Integer({0x1a770eca0ecaba9c}), Integer({0x11a4b486b4872713})},
    {Integer({0x10001}), Integer({0x1a770eca0ecaba9c}), Integer({0x88c80f88042b879})},
};

BOOST_DATA_TEST_CASE(inverse, inverse_samples)
{
    BOOST_TEST(sample.lhs.inverse(sample.rhs) == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
