#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "util.h"
#include "gear-internal.h"

using namespace grill;
using namespace gear;

BOOST_AUTO_TEST_SUITE(test_gear_internal)

struct calc_initial_inverse_sample_t {
    const std::vector<uint64_t> n;
    const initial_inverse_struct expected;
    friend std::ostream& operator<<(std::ostream& os, const calc_initial_inverse_sample_t& s) {
        os << std::hex << "n: " << gear::to_string(s.n) <<
            ", expected.value: " << s.expected.value <<
            ", expected.scale: " << s.expected.scale << std::endl;
        return os;
    }
};

static calc_initial_inverse_sample_t calc_initial_inverse_samples[] {
    {{1}, {1, 0}},
    {{1, 0}, {1, 0}},
    {{2}, {0x8000'0000'0000'0000, 1}},
    {{3}, {0x4000'0000'0000'0000, 1}},
    {{4}, {0x4000'0000'0000'0000, 1}},
    {{5}, {0x2000'0000'0000'0000, 1}},
    {{7}, {0x2000'0000'0000'0000, 1}},
    {{8}, {0x2000'0000'0000'0000, 1}},
    {{9}, {0x1000'0000'0000'0000, 1}},
    {{0x0f}, {0x1000'0000'0000'0000, 1}},
    {{0x10}, {0x1000'0000'0000'0000, 1}},
    {{0x11}, {0x0800'0000'0000'0000, 1}},
    {{0x7fff'ffff'ffff'ffff}, {0x2, 1}},
    {{0x8000'0000'0000'0000}, {0x2, 1}},
    {{0x8000'0000'0000'0001}, {0x1, 1}},
    {{0xffff'ffff'ffff'ffff}, {0x1, 1}},
    {{0, 1}, {0x1, 1}},
    {{0, 1, 0}, {0x1, 1}},
    {{1, 1}, {0x8000'0000'0000'0000, 2}},
    {{1, 1, 0}, {0x8000'0000'0000'0000, 2}},
    {{0, 2}, {0x8000'0000'0000'0000, 2}},
    {{1, 2}, {0x4000'0000'0000'0000, 2}},
};

BOOST_DATA_TEST_CASE(calc_initial_inverse, calc_initial_inverse_samples)
{
    const initial_inverse_struct result = gear::calc_initial_inverse(sample.n.data(),
                                                                     sample.n.size());
    BOOST_TEST(result.value == sample.expected.value);
    BOOST_TEST(result.scale == sample.expected.scale);
}

BOOST_AUTO_TEST_SUITE_END()
