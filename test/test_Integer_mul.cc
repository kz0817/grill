#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "constant.h"
#include "sample_types.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_Integer_mul)

static binary_op_sample_t mul_operator_samples[] {
    {Integer({0}), Integer({1}),  Integer({0})},
    {Integer({1}), Integer({1}),  Integer({1})},
    {Integer({1}), Integer({2}),  Integer({2})},
    {Integer({3}), Integer({10}), Integer({30})},
    {Integer({0xffff'ffff}),  Integer({0xffff'ffff}), Integer({0xffff'fffe'0000'0001})},

    {Integer({0x1'0000'0001}), Integer({0x2'0000'0003}),
     Integer({2, 0x5'0000'0003})},

    {Integer({0x1234'5678'ffff'ffff}), Integer({0x2468'ace0'ffff'ffff}),
     Integer({0x0296'cdb7'4f41'4c58, 0xc962fca600000001})},

    {Integer({0x0001'0002'0000'0003}), Integer({0x0002'0003'0000'0001}),
     Integer({0x2'0007'0006, 0x7'000b'0000'0003})},

    {Integer({0x1234'5678'ffff'ffff}), Integer({0x2468'ace0'ffff'ffff}),
     Integer({0x296'cdb7'4f41'4c58, 0xc962'fca6'0000'0001})},

    {Integer({0x1234'5678'9abc'def0, 0x1234'5678'9abc'def0}),
     Integer({0x1234'5678'9abc'def0, 0x1234'5678'9abc'def0}),
     Integer({0x014b'66dc'33f6'acdc, 0xa878'd649'5a92'7ab9,
              0x4d0f'77fe'1940'eedc, 0xa5e2'0890'f2a5'2100})},

    {Integer({0x1234'5678'9abc'def0, 0x1234'5678'9abc'def0}),
     Integer({0x1234'5678'9abc'def0, 0x1234'5678'9abc'def0}),
     Integer({0x014b'66dc'33f6'acdc, 0xa878'd649'5a92'7ab9,
              0x4d0f'77fe'1940'eedc, 0xa5e2'0890'f2a5'2100})},
};

BOOST_DATA_TEST_CASE(mul_unary_operator, mul_operator_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n *= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

BOOST_DATA_TEST_CASE(mul_binary_opreator, mul_operator_samples)
{
    BOOST_TEST((sample.lhs * sample.rhs) == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
