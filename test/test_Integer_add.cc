#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "constant.h"
#include "sample_types.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_Integer_add)

static binary_op_sample_t add_operator_samples[] = {
    {
        Integer({10}),
        Integer({3}),
        Integer({13})
    }, {
        Integer({MaxBlockValue}),
        Integer({1}),
        Integer({1, 0}),
    }, {
        Integer({1, 2, 3, 4}),
        Integer({10, 20}),
        Integer({1, 2, 13, 24})
    }, {
        Integer({1, MaxBlockValue, MaxBlockValue, MaxBlockValue}),
        Integer({1, 2}),
        Integer({2, 0, 1, 1})
    }, {
        Integer({1, 2}),
        Integer({10, 20, 30, 40}),
        Integer({10, 20, 31, 42})
    }, {
        Integer({1, MaxBlockValue}),
        Integer({1, 2, 3, 4}),
        Integer({1, 2, 5, 3})
    },
};

BOOST_DATA_TEST_CASE(add_unary_operator, add_operator_samples)
{
    Integer n = sample.lhs;
    BOOST_TEST((n += sample.rhs) == sample.expected);
    BOOST_TEST(n == sample.expected);
}

BOOST_DATA_TEST_CASE(add_binary_operator, add_operator_samples)
{
    BOOST_TEST((sample.lhs + sample.rhs) == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
