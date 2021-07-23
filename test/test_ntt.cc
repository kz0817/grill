#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <vector>
#include <sstream>
#include "ntt.h"

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
};

BOOST_DATA_TEST_CASE(transform_inverse, transform_samples)
{
    const std::size_t num = sample.input.size();
    uint64_t trans_out[num];
    ntt::transform(sample.input.data(), trans_out, num);

    uint64_t inv[num];
    ntt::inverse_transform(trans_out, inv, num);

    const std::vector<uint64_t> inv_vec(inv, inv + num);
    BOOST_TEST(inv_vec == sample.input);
}

BOOST_AUTO_TEST_SUITE_END()
