#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
//#include <boost/test/data/monomorphic.hpp>
#include <iostream>
#include "primality.h"

struct data_def {
    const integer& num;
    bool is_prime_number;

    friend std::ostream& operator<<(std::ostream& os, const data_def& data) {
        os << "num: " << data.num << ", is_prime_number: "
           << (data.is_prime_number ? "true" : "false");
        return os;
    }
};

static data_def data_set[] = {
    {wide_int<64>({2}), true},
    {wide_int<64>({3}), true},
    {wide_int<64>({4}), false},
};

BOOST_AUTO_TEST_SUITE(test_suite_primality)

BOOST_DATA_TEST_CASE(test_fermat_test, data_set)
{
    BOOST_TEST((primality::fermat_test(sample.num) == sample.is_prime_number));
}

BOOST_AUTO_TEST_SUITE_END()
