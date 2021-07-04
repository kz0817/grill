#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "constant.h"
#include "sample_types.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_Integer_div_mod)

static binary_op_sample_t div_operator_samples[] {
    {Integer({0}),  Integer({1}),  Integer({0})},
    {Integer({1}),  Integer({1}),  Integer({1})},
    {Integer({1}),  Integer({2}),  Integer({0})},
    {Integer({2}),  Integer({1}),  Integer({2})},
    {Integer({2}),  Integer({2}),  Integer({1})},
    {Integer({2}),  Integer({3}),  Integer({0})},
    {Integer({3}),  Integer({1}),  Integer({3})},
    {Integer({3}),  Integer({2}),  Integer({1})},
    {Integer({3}),  Integer({3}),  Integer({1})},
    {Integer({3}),  Integer({4}),  Integer({0})},
    {Integer({4}),  Integer({1}),  Integer({4})},
    {Integer({4}),  Integer({2}),  Integer({2})},
    {Integer({4}),  Integer({3}),  Integer({1})},
    {Integer({4}),  Integer({4}),  Integer({1})},
    {Integer({4}),  Integer({5}),  Integer({0})},
    {Integer({11}),  Integer({4}),  Integer({2})},
    {Integer({0x1234'5678'9abc'def0}),  Integer({0xfedc'ba98}),  Integer({0x1249'2492})},
    {Integer({0x1234'5678'9abc'def0}),  Integer({0xfedc}),  Integer({0x1249'31f5'96dc})},
    {Integer({1,0}), Integer({1}), Integer({1, 0})},
    {Integer({1,0}), Integer({2}), Integer({0x8000'0000'0000'0000})},
    {Integer({1,0}), Integer({0x0000'0002'540b'e3df}), Integer({0x6df3'7f6d})},
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
    {Integer({0}),  Integer({1}),  Integer({0})},
    {Integer({1}),  Integer({1}),  Integer({0})},
    {Integer({1}),  Integer({2}),  Integer({1})},
    {Integer({2}),  Integer({1}),  Integer({0})},
    {Integer({2}),  Integer({2}),  Integer({0})},
    {Integer({2}),  Integer({3}),  Integer({2})},
    {Integer({3}),  Integer({1}),  Integer({0})},
    {Integer({3}),  Integer({2}),  Integer({1})},
    {Integer({3}),  Integer({3}),  Integer({0})},
    {Integer({3}),  Integer({4}),  Integer({3})},
    {Integer({4}),  Integer({1}),  Integer({0})},
    {Integer({4}),  Integer({2}),  Integer({0})},
    {Integer({4}),  Integer({3}),  Integer({1})},
    {Integer({4}),  Integer({4}),  Integer({0})},
    {Integer({4}),  Integer({5}),  Integer({4})},
    {Integer({11}),  Integer({4}),  Integer({3})},
    {Integer({0x1234'5678'9abc'def0}),  Integer({0xfedc'ba98}),  Integer({0x5145'1440})},
    {Integer({0x1234'5678'9abc'def0}),  Integer({0xfedc}),  Integer({0xf1e0})},
    {Integer({1,0}), Integer({1}), Integer({0})},
    {Integer({1,0}), Integer({2}), Integer({0})},
    {Integer({1,0}), Integer({0x0000'0002'540b'e3df}), Integer({0x1'1137'590d})},
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
