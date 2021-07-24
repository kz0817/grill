#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <vector>
#include <sstream>
#include "ntt.h"
#include <iostream>

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_ntt)

template<typename T>
static std::string to_string(const std::vector<T>& vec) {
    std::ostringstream oss;
    oss << std::hex;
    std::copy(vec.begin(), vec.end()-1, std::ostream_iterator<T>(oss, ", "));
    oss << *vec.rbegin();
    return oss.str();
}

struct transform_sample_t {
    const std::vector<uint64_t> input;
    friend std::ostream& operator<<(std::ostream& os, const transform_sample_t& s) {
        os << to_string(s.input);
        return os;
    }
};

static transform_sample_t transform_samples[] {
    {{1, 2}},
    {{1, 2, 3, 4}},
    {{1, 2, 3, 4, 5, 6, 7, 8}},
    {{1, 0}},
    {{1, 0, 0, 0}},
    {{0x10'0000, 0}},
    {{0x10'0000, 0x10'0000}},
    //{{0x100'0000, 0x100'0000, 0x100'0000, 0x100'0000}},
    //{{0x100'0000, 0x100'0000}},
    {{0x100'0000, 0x0}},
};

BOOST_DATA_TEST_CASE(transform_inverse, transform_samples)
{
    const std::size_t num = sample.input.size();
    uint64_t trans_out[num];
    ntt::transform(sample.input.data(), trans_out, num);

    const std::vector<uint64_t> out_vec(trans_out, trans_out + num);
    std::cout << "out_vec: " << to_string(out_vec) << std::endl;

    uint64_t inv[num];
    ntt::inverse_transform(trans_out, inv, num);

    const std::vector<uint64_t> inv_vec(inv, inv + num);
    std::cout << "inv_vec: " << to_string(inv_vec) << std::endl;
    BOOST_TEST(inv_vec == sample.input);
}

struct split_sample_t {
    const std::vector<uint64_t> input;
    const std::size_t out_unit_bits;
    const std::vector<uint64_t> expected;
    friend std::ostream& operator<<(std::ostream& os, const split_sample_t& s) {
        os << "input: " << to_string(s.input) << ", out_unit_bits: " << s.out_unit_bits
           << ", expected: " << to_string(s.expected);
        return os;
    }
};

static split_sample_t split_samples[] {
    {{0x123456789abcdef0}, 16, {0xdef0, 0x9abc, 0x5678, 0x1234}},
    {{0x123456789abcdef0, 0xaabbccdd}, 16,
     {0xdef0, 0x9abc, 0x5678, 0x1234, 0xccdd, 0xaabb, 0, 0}},
    {{0x123456789abcdef0}, 20, {0xcdef0, 0x789ab, 0x23456, 0x1}},
    {{0x123456789abcdef0, 0xaabbccdd}, 20,
     {0xcdef0, 0x789ab, 0x23456, 0xccdd1, 0xaabb}},
};

BOOST_DATA_TEST_CASE(split, split_samples)
{
    const std::size_t n_out = sample.expected.size();
    uint64_t out[n_out];
    ntt::split(sample.input.data(), sample.input.size(), out, n_out, sample.out_unit_bits);

    const std::vector<uint64_t> out_vec(out, out + n_out);
    BOOST_TEST(out_vec == sample.expected);
}

struct ssa_sample_t {
    const std::vector<uint64_t> a;
    const std::vector<uint64_t> b;
    const std::vector<uint64_t> expected;
    friend std::ostream& operator<<(std::ostream& os, const ssa_sample_t& s) {
        os << "a: " << to_string(s.a) << ", b: " << to_string(s.b)
           << ", expected: " << to_string(s.expected);
        return os;
    }
};

static ssa_sample_t ssa_samples[] {
    {{1}, {1}, {1, 0}},
    {{1}, {2}, {2, 0}},
    {{2}, {2}, {4, 0}},
    {{0x100}, {0x100}, {0x1'0000, 0}},
    {{0x1000}, {0x1000}, {0x100'0000, 0}},
    {{0x1234}, {0x1234}, {0x14b'5a90, 0}},
    {{0x12345678}, {0x12345678}, {0x14b'66dc'1df4'd840, 0}},
};

BOOST_DATA_TEST_CASE(ssa, ssa_samples)
{
    const std::size_t n_a = sample.a.size();
    const std::size_t n_b = sample.b.size();
    const std::size_t n_out = n_a + n_b;
    uint64_t out[n_out];
    ntt::ssa(sample.a.data(), n_a, sample.b.data(), n_b, out, n_out);

    const std::vector<uint64_t> out_vec(out, out + n_out);
    std::cout << "out_vec: " << to_string(out_vec) << std::endl;
    BOOST_TEST(out_vec == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
