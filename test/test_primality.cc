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
    {Integer({2}), true},
    {Integer({3}), true},
    {Integer({4}), false},
    {Integer({5}), true},
    {Integer({6}), false},
    {Integer({7}), true},
    {Integer({8}), false},
    {Integer({9}), false},
    {Integer({10}), false},
    {Integer({11}), true},
    {Integer({12}), false},
    {Integer({13}), true},
    {Integer({14}), false},
    {Integer({15}), false},
    {Integer({16}), false},
    {Integer({17}), true},
    {Integer({18}), false},
    {Integer({19}), true},
    {Integer({20}), false},
    {Integer({21}), false},
    {Integer({22}), false},
    {Integer({23}), true},
    {Integer({24}), false},
    {Integer({25}), false},
    {Integer({26}), false},
    {Integer({27}), false},
    {Integer({28}), false},
    {Integer({29}), true},
    {Integer({127}), true},              // M7(2^7-1})
    {Integer({8191}), true, true},       // M13 (2^13-1})
    {Integer({524'287}), true, true},     // M19 (2^19-1})
    {Integer({670'0417}), true, true},    // (2^32+1})/641
    {Integer({2'147'483'647}), true, true}, // M31 (2^31-1})
    {Integer({9'999'999'967}), true, true}, // Over 2^32
    {Integer({0x7fff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}), true, true}, // M127 (2^127-1)
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
