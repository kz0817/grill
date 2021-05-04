#include <boost/test/unit_test.hpp>
#include "integer.h"

BOOST_AUTO_TEST_SUITE(test_suite_long_integer)

BOOST_AUTO_TEST_CASE(get_bytes)
{
    bitwise_integer<64> n;
    BOOST_TEST(n.get_num_blocks() == 1);
}

BOOST_AUTO_TEST_CASE(initial_value)
{
    bitwise_integer<64> n;
    BOOST_TEST(n.ref_blocks()[0] == 0);
    BOOST_TEST(static_cast<std::string>(n) == "0");
}

BOOST_AUTO_TEST_CASE(given_initial_value)
{
    const uint64_t init_value[] = {123};
    bitwise_integer<64> n(init_value);
    BOOST_TEST(n.ref_blocks()[0] == 123);
    BOOST_TEST(static_cast<std::string>(n) == "123");
}

BOOST_AUTO_TEST_CASE(given_initial_value_with_initializer_list)
{
    bitwise_integer<64> n({123});
    BOOST_TEST(n.ref_blocks()[0] == 123);
    BOOST_TEST(static_cast<std::string>(n) == "123");
}

BOOST_AUTO_TEST_CASE(add)
{
    bitwise_integer<64> n1({10});
    bitwise_integer<64> n2({3});

    n1 += n2;
    BOOST_TEST(n1.ref_blocks()[0] == 13);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(add_binary_operator)
{
    bitwise_integer<64> n1({10});
    bitwise_integer<64> n2({3});

    const auto n = n1 + n2;
    BOOST_TEST(n1.ref_blocks()[0] == 10);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
    BOOST_TEST(n.ref_blocks()[0] == 13);
}

BOOST_AUTO_TEST_CASE(sub)
{
    bitwise_integer<64> n1({10});
    bitwise_integer<64> n2({3});

    n1 -= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 7);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(sub_binary_opreator)
{
    bitwise_integer<64> n1({10});
    bitwise_integer<64> n2({3});

    const auto n = n1 - n2;
    BOOST_TEST(n1.ref_blocks()[0] == 10);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
    BOOST_TEST(n.ref_blocks()[0] == 7);
}

BOOST_AUTO_TEST_CASE(mul)
{
    bitwise_integer<64> n1({10});
    bitwise_integer<64> n2({3});

    n1 *= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 30);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(mul_binary_opreator)
{
    bitwise_integer<64> n1({10});
    bitwise_integer<64> n2({3});

    const auto n = n1 * n2;
    BOOST_TEST(n1.ref_blocks()[0] == 10);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
    BOOST_TEST(n.ref_blocks()[0] == 30);
}

BOOST_AUTO_TEST_CASE(div)
{
    bitwise_integer<64> n1({11});
    bitwise_integer<64> n2({4});

    n1 /= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 2);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
}

BOOST_AUTO_TEST_CASE(div_binary_opreator)
{
    bitwise_integer<64> n1({11});
    bitwise_integer<64> n2({4});

    const auto n = n1 / n2;
    BOOST_TEST(n1.ref_blocks()[0] == 11);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
    BOOST_TEST(n.ref_blocks()[0] == 2);
}

BOOST_AUTO_TEST_CASE(mod)
{
    bitwise_integer<64> n1({11});
    bitwise_integer<64> n2({4});

    n1 %= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 3);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
}

BOOST_AUTO_TEST_CASE(mod_binary_opreator)
{
    bitwise_integer<64> n1({11});
    bitwise_integer<64> n2({4});

    const auto n = n1 % n2;
    BOOST_TEST(n1.ref_blocks()[0] == 11);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
    BOOST_TEST(n.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_SUITE_END()
