#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "constant.h"
#include "sample_types.h"
#include "util.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_Integer_set_get_bitvalue)

struct Integer_int_to_bool_sample_t {
    const Integer& lhs;
    const int rhs;
    const bool expected;

    friend std::ostream& operator<<(std::ostream& os, const Integer_int_to_bool_sample_t& s) {
        os << "lhs: " << s.lhs << ", rhs: " << s.rhs
           << ", expected: " << util::to_string(s.expected);
        return os;
    }
};

static Integer_int_to_bool_sample_t get_bit_value_samples[] {
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  0, true},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  1, false},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 62, false},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 63, true},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  0,  false},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  63, false},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  64, true},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  65, false},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}), 126, false},
    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}), 127, true},
};

BOOST_DATA_TEST_CASE(get_bit_value, get_bit_value_samples)
{
    BOOST_TEST(sample.lhs.get_bit_value(sample.rhs) == sample.expected);
}

struct set_bit_value_sample_t {
    const Integer& n;
    const int bit;
    const bool v;
    const Integer& expected;

    friend std::ostream& operator<<(std::ostream& os, const set_bit_value_sample_t& s) {
        os << "n: " << s.n << ", bit: " << s.bit << ", v: " << util::to_string(s.v)
           << ", expected: " << s.expected;
        return os;
    }
};

static set_bit_value_sample_t set_bit_value_samples[] {
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  0, false, WideInt<64>(0xa5a5'5a5a'ffff'0000)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  0, true,  WideInt<64>(0xa5a5'5a5a'ffff'0001)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  1, false, WideInt<64>(0xa5a5'5a5a'ffff'0001)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001),  1, true,  WideInt<64>(0xa5a5'5a5a'ffff'0003)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 62, true,  WideInt<64>(0xe5a5'5a5a'ffff'0001)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 63, false, WideInt<64>(0x25a5'5a5a'ffff'0001)},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  0,  false,
     WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  1,  true,
     WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0002})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  63, true,
     WideInt<128>({0xa000'0000'0000'0001, 0xd000'0000'0000'0000})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  64, false,
     WideInt<128>({0xa000'0000'0000'0000, 0x5000'0000'0000'0000})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}),  65, true,
     WideInt<128>({0xa000'0000'0000'0003, 0x5000'0000'0000'0000})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}), 126, true,
     WideInt<128>({0xe000'0000'0000'0001, 0x5000'0000'0000'0000})},

    {WideInt<128>({0xa000'0000'0000'0001, 0x5000'0000'0000'0000}), 127, false,
     WideInt<128>({0x2000'0000'0000'0001, 0x5000'0000'0000'0000})},

    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 64, true, WideInt<128>({1, 0xa5a5'5a5a'ffff'0001})},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 64, false, WideInt<64>(0xa5a5'5a5a'ffff'0001)},
    {WideInt<64>(0xa5a5'5a5a'ffff'0001), 128, true, WideInt<192>({1, 0, 0xa5a5'5a5a'ffff'0001})},
};

BOOST_DATA_TEST_CASE(set_bit_value, set_bit_value_samples)
{
    Integer n(sample.n);
    BOOST_TEST(n.set_bit_value(sample.bit, sample.v) == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
