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
    {WideInt<128>({1,0}), WideInt<64>(1), WideInt<128>({1, 0})},
    {WideInt<128>({1,0}), WideInt<64>(2), WideInt<128>({0x8000'0000'0000'0000})},
    {WideInt<128>({1,0}), WideInt<64>(0x0000'0002'540b'e3df), WideInt<128>({0x6df3'7f6d})},
};

BOOST_DATA_TEST_CASE(div_unary_operator, div_operator_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n /= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
    BOOST_TEST(n.get_num_blocks() == sample.expected.get_num_blocks());
}

BOOST_DATA_TEST_CASE(div_binary_opreator, div_operator_samples)
{
    const Integer n = sample.lhs / sample.rhs;
    BOOST_TEST(n == sample.expected);
    BOOST_TEST(n.get_num_blocks() == sample.expected.get_num_blocks());
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
    {WideInt<128>({1,0}), WideInt<64>(1), WideInt<128>({0})},
    {WideInt<128>({1,0}), WideInt<64>(2), WideInt<128>({0})},
    {WideInt<128>({1,0}), WideInt<64>(0x0000'0002'540b'e3df), WideInt<128>({0x1'1137'590d})},
};

BOOST_DATA_TEST_CASE(mod_unary_operator, mod_operator_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n %= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
    BOOST_TEST(n.get_num_blocks() == sample.expected.get_num_blocks());
}

BOOST_DATA_TEST_CASE(mod_binary_opreator, mod_operator_samples)
{
    const Integer n = sample.lhs % sample.rhs;
    BOOST_TEST(n == sample.expected);
    BOOST_TEST(n.get_num_blocks() == sample.expected.get_num_blocks());
}

BOOST_AUTO_TEST_SUITE_END()