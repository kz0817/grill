#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <iostream>
#include "primality.h"

BOOST_AUTO_TEST_SUITE(test_suite_primality)

static struct sample_t {
    const integer& num;
    bool is_prime_number;

    friend std::ostream& operator<<(std::ostream& os, const sample_t& s) {
        os << "num: " << s.num << ", is_prime_number: "
           << (s.is_prime_number ? "true" : "false");
        return os;
    }
} samples[] = {
    {wide_int<64>(2), true},
    {wide_int<64>(3), true},
    {wide_int<64>(4), false},
    {wide_int<64>(5), true},
    {wide_int<64>(6), false},
    {wide_int<64>(7), true},
    {wide_int<64>(8), false},
    {wide_int<64>(9), false},
    {wide_int<64>(10), false},
    {wide_int<64>(11), true},
    {wide_int<64>(12), false},
};

BOOST_DATA_TEST_CASE(test_fermat_test, samples)
{
    BOOST_TEST(primality::fermat_test(sample.num) == sample.is_prime_number);
}

BOOST_AUTO_TEST_SUITE_END()
