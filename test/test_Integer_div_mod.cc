#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "constant.h"
#include "sample_types.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_Integer_div_mod)

static binary_op_sample_t div_operator_samples[] {
    {WideInt<64>(0),  WideInt<64>(1),  WideInt<64>(0)},
    {WideInt<64>(1),  WideInt<64>(1),  WideInt<64>(1)},
    {WideInt<64>(1),  WideInt<64>(2),  WideInt<64>(0)},
    {WideInt<64>(2),  WideInt<64>(1),  WideInt<64>(2)},
    {WideInt<64>(2),  WideInt<64>(2),  WideInt<64>(1)},
    {WideInt<64>(2),  WideInt<64>(3),  WideInt<64>(0)},
    {WideInt<64>(3),  WideInt<64>(1),  WideInt<64>(3)},
    {WideInt<64>(3),  WideInt<64>(2),  WideInt<64>(1)},
    {WideInt<64>(3),  WideInt<64>(3),  WideInt<64>(1)},
    {WideInt<64>(3),  WideInt<64>(4),  WideInt<64>(0)},
    {WideInt<64>(4),  WideInt<64>(1),  WideInt<64>(4)},
    {WideInt<64>(4),  WideInt<64>(2),  WideInt<64>(2)},
    {WideInt<64>(4),  WideInt<64>(3),  WideInt<64>(1)},
    {WideInt<64>(4),  WideInt<64>(4),  WideInt<64>(1)},
    {WideInt<64>(4),  WideInt<64>(5),  WideInt<64>(0)},
    {WideInt<64>(11),  WideInt<64>(4),  WideInt<64>(2)},
    {WideInt<64>(0x1234'5678'9abc'def0),  WideInt<64>(0xfedc'ba98),  WideInt<64>(0x1249'2492)},
    {WideInt<64>(0x1234'5678'9abc'def0),  WideInt<64>(0xfedc),  WideInt<64>(0x1249'31f5'96dc)},
    // TODO: add samples with greater digits
};

BOOST_DATA_TEST_CASE(div_unary_operator, div_operator_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n /= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

BOOST_DATA_TEST_CASE(div_binary_opreator, div_operator_samples)
{
    BOOST_TEST((sample.lhs / sample.rhs) == sample.expected);
}

BOOST_AUTO_TEST_CASE(div_by_zero)
{
    BOOST_CHECK_THROW(constant::One / constant::Zero, std::out_of_range);
}

static binary_op_sample_t mod_operator_samples[] {
    {WideInt<64>(0),  WideInt<64>(1),  WideInt<64>(0)},
    {WideInt<64>(1),  WideInt<64>(1),  WideInt<64>(0)},
    {WideInt<64>(1),  WideInt<64>(2),  WideInt<64>(1)},
    {WideInt<64>(2),  WideInt<64>(1),  WideInt<64>(0)},
    {WideInt<64>(2),  WideInt<64>(2),  WideInt<64>(0)},
    {WideInt<64>(2),  WideInt<64>(3),  WideInt<64>(2)},
    {WideInt<64>(3),  WideInt<64>(1),  WideInt<64>(0)},
    {WideInt<64>(3),  WideInt<64>(2),  WideInt<64>(1)},
    {WideInt<64>(3),  WideInt<64>(3),  WideInt<64>(0)},
    {WideInt<64>(3),  WideInt<64>(4),  WideInt<64>(3)},
    {WideInt<64>(4),  WideInt<64>(1),  WideInt<64>(0)},
    {WideInt<64>(4),  WideInt<64>(2),  WideInt<64>(0)},
    {WideInt<64>(4),  WideInt<64>(3),  WideInt<64>(1)},
    {WideInt<64>(4),  WideInt<64>(4),  WideInt<64>(0)},
    {WideInt<64>(4),  WideInt<64>(5),  WideInt<64>(4)},
    {WideInt<64>(11),  WideInt<64>(4),  WideInt<64>(3)},
    {WideInt<64>(0x1234'5678'9abc'def0),  WideInt<64>(0xfedc'ba98),  WideInt<64>(0x5145'1440)},
    {WideInt<64>(0x1234'5678'9abc'def0),  WideInt<64>(0xfedc),  WideInt<64>(0xf1e0)},
    // TODO: add samples with greater digits
};

BOOST_DATA_TEST_CASE(mod_unary_operator, mod_operator_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n %= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

BOOST_DATA_TEST_CASE(mod_binary_opreator, mod_operator_samples)
{
    BOOST_TEST((sample.lhs % sample.rhs) == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
