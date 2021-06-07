#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <iostream>
#include "primality.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_primality)

static struct sample_t {
    const integer& num;
    bool is_prime_number;

    friend std::ostream& operator<<(std::ostream& os, const sample_t& s) {
        os << "num: " << s.num << ", is_prime_number: " << util::to_string(s.is_prime_number);
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
    {wide_int<64>(13), true},
    {wide_int<64>(14), false},
    {wide_int<64>(15), false},
    {wide_int<64>(16), false},
    {wide_int<64>(17), true},
    {wide_int<64>(18), false},
    {wide_int<64>(19), true},
    {wide_int<64>(20), false},
    {wide_int<64>(21), false},
    {wide_int<64>(22), false},
    {wide_int<64>(23), true},
    {wide_int<64>(24), false},
    {wide_int<64>(25), false},
    {wide_int<64>(26), false},
    {wide_int<64>(27), false},
    {wide_int<64>(28), false},
    {wide_int<64>(29), true},
};

BOOST_DATA_TEST_CASE(test_trivial_division_for_native, samples)
{
    const integer::block_t n = sample.num.ref_blocks()[0];
    BOOST_TEST(primality::trivial_division(n) == sample.is_prime_number);
}

BOOST_DATA_TEST_CASE(test_trivial_division, samples)
{
    BOOST_TEST(primality::trivial_division(sample.num) == sample.is_prime_number);
}

BOOST_DATA_TEST_CASE(test_fermat_test, samples)
{
    BOOST_TEST(primality::fermat_test(sample.num) == sample.is_prime_number);
}

BOOST_DATA_TEST_CASE(test_miller_rabin_test, samples)
{
    BOOST_TEST(primality::miller_rabin_test(sample.num) == sample.is_prime_number);
}

BOOST_AUTO_TEST_SUITE_END()
