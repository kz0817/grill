#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "util.h"
#include "gear.h"
#include "test-funcs.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_gear)

struct mul_sample_t {
    const uint64_t in0, in1;
    const uint64_t expected[2];
    const std::vector<uint64_t> input;
    friend std::ostream& operator<<(std::ostream& os, const mul_sample_t& s) {
        os << std::hex << "in0: " << s.in0 << ", in1: " << s.in1 <<
             ", expected[0]: " << s.expected[0] << ", expected:[1]" << s.expected[1] << std::endl;
        return os;
    }
};

static mul_sample_t mul_samples[] {
    {1, 1, {1, 0}},
    {0xffff'ffff, 0xffff'ffff, {0xffff'fffe'0000'0001, 0}},
    {0xffff'ffff, 0xffff'ffff'ffff'ffff, {0xffff'ffff'0000'0001, 0xffff'fffe}},
    {0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff, {0x0000'0000'0000'0001, 0xffff'ffff'ffff'fffe}},
};

BOOST_DATA_TEST_CASE(mul, mul_samples)
{
    uint64_t out[2];
    gear::mul(out, sample.in0, sample.in1);
    BOOST_TEST(out[0] == sample.expected[0]);
    BOOST_TEST(out[1] == sample.expected[1]);
}

struct add_sub_sample_t {
    const std::vector<uint64_t> lhs;
    const std::vector<uint64_t> rhs;
    const std::vector<uint64_t> expected;
    const bool expected_carry;
    friend std::ostream& operator<<(std::ostream& os, const add_sub_sample_t& s) {
        os << std::hex << "lhs: " << gear::to_string(s.lhs) <<
            ", rhs: " << gear::to_string(s.rhs) <<
            ", expected: " << gear::to_string(s.expected) <<
            ", expected_carry: " << util::to_string(s.expected_carry) << std::endl;
        return os;
    }
};

static add_sub_sample_t add_samples[] {
    {{1}, {2}, {3}, false},
    {{0xffff'ffff'ffff'ffff}, {1}, {0}, true},
    {{0xffff'ffff'ffff'ffff, 0}, {1}, {0, 1}, false},
    {{0xffff'ffff'ffff'ffff, 0}, {0xffff'ffff'ffff'ffff, 0}, {0xffff'ffff'ffff'fffe, 1}, false},
    {{0xffff'ffff'ffff'ffff}, {0xffff'ffff'ffff'ffff}, {0xffff'ffff'ffff'fffe}, true},

    {{0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff, 0x8000'0000'0000'0000},
     {0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff},
     {0xffff'ffff'ffff'fffe, 0xffff'ffff'ffff'ffff, 0x8000'0000'0000'0001}, false},

    {{0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'fffe, 0xffff'ffff'ffff'fffe}, {2, 3, 4},
     {1, 2, 3}, true},
};

BOOST_DATA_TEST_CASE(add, add_samples)
{
    std::vector<uint64_t> lhs = sample.lhs;
    const bool carry = gear::add(lhs.data(), lhs.size(), sample.rhs.data(), sample.rhs.size());
    BOOST_TEST(lhs == sample.expected);
    BOOST_TEST(carry == sample.expected_carry);
}

static add_sub_sample_t sub_samples[] {
    {{3}, {2}, {1}, false},
    {{1}, {2}, {0xffff'ffff'ffff'ffff}, true},
    {{9, 8}, {2, 3}, {7, 5}, false},
    {{2, 5}, {3, 1}, {0xffff'ffff'ffff'ffff, 3}, false},
    {{0, 0}, {1, 1}, {0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'fffe}, true},
    {{0, 0}, {2}, {0xffff'ffff'ffff'fffe, 0xffff'ffff'ffff'ffff}, true},
    {{0x8000'0000'0000'0000}, {}, {0x8000'0000'0000'0000}, false},
};

BOOST_DATA_TEST_CASE(sub, sub_samples)
{
    std::vector<uint64_t> lhs = sample.lhs;
    const bool carry = gear::sub(lhs.data(), lhs.size(), sample.rhs.data(), sample.rhs.size());
    BOOST_TEST(lhs == sample.expected);
    BOOST_TEST(carry == sample.expected_carry);
}

struct twos_complement_sample_t {
    const std::vector<uint64_t> buf;
    const std::vector<uint64_t> expected;
    friend std::ostream& operator<<(std::ostream& os, const twos_complement_sample_t& s) {
        os << std::hex << "buf: " << gear::to_string(s.buf) <<
             ", expected: " << gear::to_string(s.expected) << std::endl;
        return os;
    }
};

static twos_complement_sample_t twos_complement_samples[] {
    {{1}, {0xffff'ffff'ffff'ffff}},
    {{0xffff'ffff'ffff'ffff}, {1}},
    {{0xffff'ffff'ffff'ffff, 0xffff'ffff'ffff'ffff}, {1, 0}},

    {{0x0000'0000'0000'0001, 0x8000'0000'0000'0000},
     {0xffff'ffff'ffff'ffff, 0x7fff'ffff'ffff'ffff}},

    {{0xffff'ffff'ffff'ffff, 0x7fff'ffff'ffff'ffff},
     {0x0000'0000'0000'0001, 0x8000'0000'0000'0000}},
};

BOOST_DATA_TEST_CASE(twos_complement, twos_complement_samples)
{
    std::vector<uint64_t> buf = sample.buf;
    gear::twos_complement(buf.data(), buf.size());
    BOOST_TEST(buf == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
