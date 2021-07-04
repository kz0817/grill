#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "constant.h"
#include "sample_types.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_Integer_sub)

static binary_op_sample_t sub_operator_samples[] = {
    {
        Integer({10}),
        Integer({3}),
        Integer({7}),
    }, {
        constant::Zero,
        Integer({1}),
        Integer({MaxBlockValue}),
    }, {
        Integer({1, 2, 13, 24}),
        Integer({1, 2, 3, 4}),
        Integer({10, 20}),
    }, {
        Integer({2, 0, 1, 1}),
        Integer({1, 2}),
        Integer({1, MaxBlockValue, MaxBlockValue, MaxBlockValue}),
    }, {
        Integer({31, 42}),
        Integer({10, 20, 30, 40}),
        Integer({1, 2}),
    }, {
        Integer({5, 3}),
        Integer({1, 2, 3, 4}),
        Integer({1, MaxBlockValue}),
    },
};

BOOST_DATA_TEST_CASE(sub_unary_operator, sub_operator_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n -= sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
    BOOST_TEST(n.get_num_blocks() == sample.expected.get_num_blocks());
}

BOOST_DATA_TEST_CASE(sub_binary_opreator, sub_operator_samples)
{
    Integer result = sample.lhs - sample.rhs;
    BOOST_TEST(result == sample.expected);
    BOOST_TEST(result.get_num_blocks() == sample.expected.get_num_blocks());
}

BOOST_AUTO_TEST_SUITE_END()
