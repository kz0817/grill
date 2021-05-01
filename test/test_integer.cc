#include <boost/test/unit_test.hpp>
#include "integer.h"

BOOST_AUTO_TEST_SUITE(test_suite_long_integer)

BOOST_AUTO_TEST_CASE(get_bytes)
{
    bitwise_integer<64> n;
    BOOST_TEST(n.num_buf() == 1);
}

BOOST_AUTO_TEST_CASE(initial_value)
{
    bitwise_integer<64> n;
    BOOST_TEST(n.ref_buf()[0] == 0);
    BOOST_TEST(static_cast<std::string>(n) == "0");
}

BOOST_AUTO_TEST_SUITE_END()
