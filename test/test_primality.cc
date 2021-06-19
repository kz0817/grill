#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <iostream>
#include "primality.h"
#include "util.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_primality)

static struct sample_t {
    const Integer& num;
    bool is_prime_number;
    bool skip_trivial_div = false;

    friend std::ostream& operator<<(std::ostream& os, const sample_t& s) {
        os << "num: " << s.num << ", is_prime_number: " << util::to_string(s.is_prime_number);
        return os;
    }
} samples[] = {
    {WideInt<64>(2), true},
    {WideInt<64>(3), true},
    {WideInt<64>(4), false},
    {WideInt<64>(5), true},
    {WideInt<64>(6), false},
    {WideInt<64>(7), true},
    {WideInt<64>(8), false},
    {WideInt<64>(9), false},
    {WideInt<64>(10), false},
    {WideInt<64>(11), true},
    {WideInt<64>(12), false},
    {WideInt<64>(13), true},
    {WideInt<64>(14), false},
    {WideInt<64>(15), false},
    {WideInt<64>(16), false},
    {WideInt<64>(17), true},
    {WideInt<64>(18), false},
    {WideInt<64>(19), true},
    {WideInt<64>(20), false},
    {WideInt<64>(21), false},
    {WideInt<64>(22), false},
    {WideInt<64>(23), true},
    {WideInt<64>(24), false},
    {WideInt<64>(25), false},
    {WideInt<64>(26), false},
    {WideInt<64>(27), false},
    {WideInt<64>(28), false},
    {WideInt<64>(29), true},
    {WideInt<64>(127), true},              // M7(2^7-1)
    {WideInt<64>(8191), true, true},       // M13 (2^13-1)
    {WideInt<64>(524'287), true, true},     // M19 (2^19-1)
    {WideInt<64>(670'0417), true, true},    // (2^32+1)/641
    {WideInt<64>(2'147'483'647), true, true}, // M31 (2^31-1)
};

BOOST_DATA_TEST_CASE(test_trivial_division_for_native, samples)
{
    if (sample.skip_trivial_div)
        return;
    const Integer::block_t n = sample.num.ref_blocks()[0];
    BOOST_TEST(primality::trivial_division(n) == sample.is_prime_number);
}

BOOST_DATA_TEST_CASE(test_trivial_division, samples)
{
    if (sample.skip_trivial_div)
        return;
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
