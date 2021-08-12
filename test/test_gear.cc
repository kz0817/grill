#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
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
    friend std::ostream& operator<<(std::ostream& os, const add_sub_sample_t& s) {
        os << std::hex << "lhs: " << to_string(s.lhs) << ", rhs: " << to_string(s.rhs) <<
             ", expected: " << to_string(s.expected) << std::endl;
        return os;
    }
};

static add_sub_sample_t add_samples[] {
    {{1}, {2}, {3}},
    {{0xffff'ffff'ffff'ffff}, {1}, {0}},
    {{0, 0xffff'ffff'ffff'ffff}, {1}, {1, 0}},
};

BOOST_DATA_TEST_CASE(add, add_samples)
{
    std::vector<uint64_t> lhs = sample.lhs;
    gear::add(lhs.data(), lhs.size(), sample.rhs.data(), sample.rhs.size());
    BOOST_TEST(lhs == sample.expected);
}

BOOST_AUTO_TEST_SUITE_END()
