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

struct karatsuba_sample_t {
    const std::vector<uint64_t> in0;
    const std::vector<uint64_t> in1;
    const std::vector<uint64_t> expected;
    friend std::ostream& operator<<(std::ostream& os, const karatsuba_sample_t& s) {
        os << std::hex << "in0: " << gear::to_string(s.in0) <<
            ", in1: " << gear::to_string(s.in1) <<
            ", expected: " << gear::to_string(s.expected) << std::endl;
        return os;
    }
};

static karatsuba_sample_t karatsuba_samples[] {
    {{1}, {2}, {2, 0}},
    {{0x8000'0000'0000'0000}, {0x8000'0000'0000'0000}, {0, 0x4000'0000'0000'0000}},

    {{0x8000'0000'0000'0000, 1}, {0x8000'0000'0000'0000},
    {0, 0xc000'0000'0000'0000, 0}},

    {{0, 1}, {1}, {0, 1}},
    {{0, 1}, {1}, {0, 1, 0}},
    {{1, 3}, {4, 2}, {4, 14, 6, 0}},

    {{0x8000'0000'0000'0000, 0xa000'0000'0000'0000},
     {0x9000'0000'0000'0000, 0xb000'0000'0000'0000},
     {0x0000'0000'0000'0000, 0x4800'0000'0000'0000, 0xb200'0000'0000'0000, 0x6e00'0000'0000'0000}},

    {{0x1234'5678'9abc'def0, 0x1234'5678'9abc'def0},
     {0xfedc'ba98'7654'3210, 0xfedc'ba98'7654'3210},
     {0x236d'88fe'5618'cf00, 0x58fa'b207'83af'1222, 0x47ac'c914'0513'b744, 0x121f'a00a'd77d'7422}},

    {{1, 2, 3, 4}, {5, 6, 7, 8}, {0x5, 0x10, 0x22, 0x3c, 0x3d, 0x34, 0x20, 0}},
    {{1, 2, 3, 4}, {7, 8}, {0x7, 0x16, 0x25, 0x34, 0x20, 0}},
};

BOOST_DATA_TEST_CASE(karatsuba, karatsuba_samples)
{
    std::vector<uint64_t> out(sample.expected.size());
    gear::karatsuba(out.data(), out.size(),
                    sample.in0.data(), sample.in0.size(), sample.in1.data(), sample.in1.size());
    BOOST_TEST(out == sample.expected);
}

struct get_first_non_zero_index_sample_t {
    const std::vector<std::uint64_t> n;
    const std::size_t expected;
    friend std::ostream& operator<<(std::ostream& os, const get_first_non_zero_index_sample_t& s) {
        os << std::hex << "n: " << gear::to_string(s.n) <<
            ", expected: " << s.expected << std::endl;
        return os;
    }
};

static get_first_non_zero_index_sample_t get_first_non_zero_index_samples[] {
    {{0}, gear::NotFound},
    {{0, 0}, gear::NotFound},
    {{1}, 0},
    {{0, 1}, 1},
    {{0, 1, 0}, 1},
    {{0, 0, 3}, 2},
};

BOOST_DATA_TEST_CASE(get_first_non_zero_index, get_first_non_zero_index_samples)
{
    const std::uint64_t result = gear::get_first_non_zero_index(sample.n.data(), sample.n.size());
    BOOST_TEST(result == sample.expected);
}

struct get_most_significant_active_bit_sample_t {
    const std::uint64_t n;
    const std::uint64_t expected;
    friend std::ostream& operator<<(std::ostream& os,
                                    const get_most_significant_active_bit_sample_t& s) {
        os << std::hex << "n: " << s.n << ", expected: " << s.expected << std::endl;
        return os;
    }
};

static get_most_significant_active_bit_sample_t get_most_significant_active_bit_samples[] {
    {0x0000'0000'0000'0000, gear::NotFound},
    {0x0000'0000'0000'0001, 0},
    {0x0000'0000'0000'0002, 1},
    {0x0000'0000'0000'0003, 1},
    {0x0000'0000'8765'4321, 31},
    {0x7fff'ffff'ffff'ffff, 62},
    {0x8000'0000'0000'0000, 63},
    {0xffff'ffff'ffff'ffff, 63},
};

BOOST_DATA_TEST_CASE(get_most_significant_active_bit, get_most_significant_active_bit_samples)
{
    const std::uint64_t result = gear::get_most_significant_active_bit(sample.n);
    BOOST_TEST(result == sample.expected);
}

struct get_leading_identical_bits_sample_t {
    const std::uint64_t n0;
    const std::uint64_t n1;
    const std::uint64_t expected;
    friend std::ostream& operator<<(std::ostream& os,
                                    const get_leading_identical_bits_sample_t& s) {
        os << std::hex << "n0: " << s.n0 << ", n1: " << s.n1 <<
            ", expected: " << s.expected << std::endl;
        return os;
    }
};

static get_leading_identical_bits_sample_t get_leading_identical_bits_samples[] {
    {0x8000'0000'0000'0000, 0x0000'0000'0000'0000, 0},
    {0x8000'0000'0000'0000, 0x4000'0000'0000'0000, 0},
    {0x8000'0000'0000'0000, 0xc000'0000'0000'0000, 1},
    {0x8000'0000'0000'0000, 0xa000'0000'0000'0000, 2},
    {0x8000'1234'0000'5678, 0x8000'1234'0000'5679, 63},
    {0x8000'1234'0000'5678, 0x8000'1234'0000'5678, 64},
};

BOOST_DATA_TEST_CASE(get_leading_identical_bits, get_leading_identical_bits_samples)
{
    const std::uint64_t result = gear::get_leading_identical_bits(sample.n0, sample.n1);
    BOOST_TEST(result == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
