#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <iostream>
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

struct inverse_sample_t {
    const std::vector<uint64_t> in;
    const std::size_t significant_bits;
    const std::vector<uint64_t> expected;
    const std::size_t expected_out_scale;
    friend std::ostream& operator<<(std::ostream& os, const inverse_sample_t& s) {
        os << std::hex << "n0: " << gear::to_string(s.in) <<
            ", significant_bits: " << s.significant_bits <<
            ", expected: " << gear::to_string(s.expected) <<
            ", expected_out_scale: " << s.expected_out_scale << std::endl;
        return os;
    }
};

static inverse_sample_t inverse_samples[] {
    {{2}, 1, {0x8000'0000'0000'0000}, 1},
    {{2}, 2, {0x8000'0000'0000'0000}, 1},
    {{2}, 8, {0x8000'0000'0000'0000}, 1},
    {{2}, 32, {0x8000'0000'0000'0000}, 1},
    {{2}, 64, {0x8000'0000'0000'0000}, 1},
};

static void clear_under_significant_bits(uint64_t* out, const uint64_t* in , const std::size_t n,
                                         const std::size_t significant_bits) {
    constexpr std::size_t Uint64Bits = 64;
    assert(significant_bits > 0);
    const std::size_t num_significant_block = ((significant_bits-1) / Uint64Bits);
    assert(num_significant_block < n);
    const std::size_t significant_block_idx = (n - 1) - num_significant_block;

    const std::size_t mask_bits = (significant_bits-1) % Uint64Bits;
    const uint64_t mask = -(0x8000'0000'0000'000 >> mask_bits);

    for (std::size_t i = 0; i < n; i++) {
        if (i < significant_block_idx)
            out[i] = 0;
        else if (i > significant_block_idx)
            out[i] = in[i];
        else
            out[i] = in[i] & mask;
    }
}

static std::vector<uint64_t> create_inverse_expected_value(const std::vector<uint64_t>& in,
                                                           const std::size_t significant_bits) {
    uint64_t buf[in.size()];
    clear_under_significant_bits(buf, in.data(), in.size(), significant_bits);
    return std::vector(buf, buf + in.size());
}

static std::vector<uint64_t> create_minus1_expected_value(const std::vector<uint64_t>& in,
                                                          const std::size_t significant_bits) {
    uint64_t buf[in.size()];
    const uint64_t One = 1;
    gear::copy(buf, in.data(), in.size());
    gear::sub(buf, in.size(), &One, 1);
    return create_inverse_expected_value(std::vector(buf, buf + in.size()), significant_bits);
}

BOOST_DATA_TEST_CASE(inverse, inverse_samples)
{
    std::vector<uint64_t> raw_out(sample.expected.size());
    const std::size_t out_scale = gear::inverse(raw_out.data(), raw_out.size(),
                                                sample.in.data(), sample.in.size(),
                                                sample.significant_bits);
    const std::vector<uint64_t> expected_minus1 =
        create_minus1_expected_value(sample.expected, sample.significant_bits);

    const std::vector<uint64_t> fixed_out =
        create_inverse_expected_value(raw_out, sample.significant_bits);

    if (is_verbose()) {
        std::cerr << "fixed_out: " << gear::to_string(fixed_out) <<
            ", expected_minus1: " << gear::to_string(expected_minus1) << std::endl;
    }

    BOOST_CHECK((fixed_out == sample.expected) || (fixed_out == expected_minus1));
    BOOST_TEST(out_scale == sample.expected_out_scale);
}

BOOST_AUTO_TEST_SUITE_END()
