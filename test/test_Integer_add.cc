#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "constant.h"
#include "sample_types.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_Integer_add)

static binary_op_sample_t add_operator_samples[] = {
    {
        WideInt<64>(10),
        WideInt<64>(3),
        WideInt<64>(13)
    }, {
        WideInt<64>(MaxBlockValue),
        WideInt<64>(1),
        WideInt<128>({1, 0}),
    }, {
        WideInt<256>({1, 2, 3, 4}),
        WideInt<128>({10, 20}),
        WideInt<256>({1, 2, 13, 24})
    }, {
        WideInt<256>({1, MaxBlockValue, MaxBlockValue, MaxBlockValue}),
        WideInt<128>({1, 2}),
        WideInt<256>({2, 0, 1, 1})
    }, {
        WideInt<128>({1, 2}),
        WideInt<256>({10, 20, 30, 40}),
        WideInt<256>({10, 20, 31, 42})
    }, {
        WideInt<128>({1, MaxBlockValue}),
        WideInt<256>({1, 2, 3, 4}),
        WideInt<256>({1, 2, 5, 3})
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
