#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "sample_types.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_Integer_inverse)

static binary_op_sample_t inverse_samples[] = {
    {WideInt<64>(1), WideInt<64>(5), WideInt<64>(1)},
    {WideInt<64>(2), WideInt<64>(5), WideInt<64>(3)},
    {WideInt<64>(3), WideInt<64>(5), WideInt<64>(2)},
    {WideInt<64>(4), WideInt<64>(5), WideInt<64>(4)},

    {WideInt<64>(6), WideInt<64>(5), WideInt<64>(1)},
    {WideInt<64>(7), WideInt<64>(5), WideInt<64>(3)},
    {WideInt<64>(8), WideInt<64>(5), WideInt<64>(2)},
    {WideInt<64>(9), WideInt<64>(5), WideInt<64>(4)},

    // TODO: Add samples with greater digits.
    {WideInt<64>(1), WideInt<64>(0x1a770ecab58cfb0d), WideInt<64>(1)},
    {WideInt<64>(3), WideInt<64>(0x1a770ecab58cfb0d), WideInt<64>(0x11a4b48723b35209)},
    {WideInt<64>(3), WideInt<64>(0x1a770eca0ecaba9c), WideInt<64>(0x11a4b486b4872713)},
    {WideInt<64>(0x10001), WideInt<64>(0x1a770eca0ecaba9c), WideInt<64>(0x88c80f88042b879)},
};

BOOST_DATA_TEST_CASE(inverse, inverse_samples)
{
    BOOST_TEST(sample.lhs.inverse(sample.rhs) == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
