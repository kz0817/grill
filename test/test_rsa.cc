#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "sample_types.h"
#include "rsa.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_rsa)

struct binary_mod_op_sample_t {
    const grill::Integer& mod;
    const grill::Integer& pub_exp;
    const grill::Integer& priv_exp;
    const grill::Integer& n;
    const grill::Integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const binary_mod_op_sample_t& s) {
        os << "pub_exp: " << s.pub_exp << ", priv_exp: " << s.priv_exp
           << ", mod: " << s.mod << ", n: " << s.n << ", expected: " << s.expected;
        return os;
    }
};

static binary_mod_op_sample_t compute_samples[] {
    // p=7, q=11, n=77, phi=6*10=60, pub=13, priv=37
    {Integer({77}), Integer({13}), Integer({37}),
     Integer({1}), Integer({1})},
    {Integer({77}), Integer({13}), Integer({37}),
     Integer({2}), Integer({30})},
    {Integer({77}), Integer({13}), Integer({37}),
     Integer({3}), Integer({38})},
    {Integer({77}), Integer({13}), Integer({37}),
     Integer({4}), Integer({53})},
};

BOOST_DATA_TEST_CASE(compute, compute_samples)
{
    const Integer& n = sample.n;
    const Integer& modulus = sample.mod;
    const Integer encrypted = rsa::compute(sample.pub_exp, modulus, n);
    BOOST_TEST(encrypted == sample.expected);
    BOOST_TEST(rsa::compute(sample.priv_exp, modulus, encrypted) == n);
}

BOOST_AUTO_TEST_SUITE_END()
