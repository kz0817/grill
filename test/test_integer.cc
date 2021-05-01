#include <boost/test/unit_test.hpp>
#include "integer.h"

BOOST_AUTO_TEST_SUITE(test_suite_long_integer)

BOOST_AUTO_TEST_CASE(get_bytes)
{
    bitwise_integer<64> n;
    BOOST_TEST(n.num_chunks() == 1);
}

BOOST_AUTO_TEST_CASE(initial_value)
{
    bitwise_integer<64> n;
    BOOST_TEST(n.ref_chunks()[0] == 0);
    BOOST_TEST(static_cast<std::string>(n) == "0");
}

BOOST_AUTO_TEST_CASE(given_initial_value)
{
    const uint64_t init_value[] = {123};
    bitwise_integer<64> n(init_value);
    BOOST_TEST(n.ref_chunks()[0] == 123);
    BOOST_TEST(static_cast<std::string>(n) == "123");
}

BOOST_AUTO_TEST_CASE(add)
{
    const uint64_t init_value1[] = {10};
    bitwise_integer<64> n1(init_value1);

    const uint64_t init_value2[] = {3};
    bitwise_integer<64> n2(init_value2);

    n1 += n2;
    BOOST_TEST(n1.ref_chunks()[0] == 13);
    BOOST_TEST(n2.ref_chunks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(add_binary_operator)
{
    const uint64_t init_value1[] = {10};
    bitwise_integer<64> n1(init_value1);

    const uint64_t init_value2[] = {3};
    bitwise_integer<64> n2(init_value2);

    const auto n = n1 + n2;
    BOOST_TEST(n1.ref_chunks()[0] == 10);
    BOOST_TEST(n2.ref_chunks()[0] == 3);
    BOOST_TEST(n.ref_chunks()[0] == 13);
}

BOOST_AUTO_TEST_CASE(sub)
{
    const uint64_t init_value1[] = {10};
    bitwise_integer<64> n1(init_value1);

    const uint64_t init_value2[] = {3};
    bitwise_integer<64> n2(init_value2);

    n1 -= n2;
    BOOST_TEST(n1.ref_chunks()[0] == 7);
    BOOST_TEST(n2.ref_chunks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(sub_binary_opreator)
{
    const uint64_t init_value1[] = {10};
    bitwise_integer<64> n1(init_value1);

    const uint64_t init_value2[] = {3};
    bitwise_integer<64> n2(init_value2);

    const auto n = n1 - n2;
    BOOST_TEST(n1.ref_chunks()[0] == 10);
    BOOST_TEST(n2.ref_chunks()[0] == 3);
    BOOST_TEST(n.ref_chunks()[0] == 7);
}

BOOST_AUTO_TEST_CASE(mul)
{
    const uint64_t init_value1[] = {10};
    bitwise_integer<64> n1(init_value1);

    const uint64_t init_value2[] = {3};
    bitwise_integer<64> n2(init_value2);

    n1 *= n2;
    BOOST_TEST(n1.ref_chunks()[0] == 30);
    BOOST_TEST(n2.ref_chunks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(mul_binary_opreator)
{
    const uint64_t init_value1[] = {10};
    bitwise_integer<64> n1(init_value1);

    const uint64_t init_value2[] = {3};
    bitwise_integer<64> n2(init_value2);

    const auto n = n1 * n2;
    BOOST_TEST(n1.ref_chunks()[0] == 10);
    BOOST_TEST(n2.ref_chunks()[0] == 3);
    BOOST_TEST(n.ref_chunks()[0] == 30);
}

BOOST_AUTO_TEST_CASE(div)
{
    const uint64_t init_value1[] = {11};
    bitwise_integer<64> n1(init_value1);

    const uint64_t init_value2[] = {4};
    bitwise_integer<64> n2(init_value2);

    n1 /= n2;
    BOOST_TEST(n1.ref_chunks()[0] == 2);
    BOOST_TEST(n2.ref_chunks()[0] == 4);
}

BOOST_AUTO_TEST_CASE(div_binary_opreator)
{
    const uint64_t init_value1[] = {11};
    bitwise_integer<64> n1(init_value1);

    const uint64_t init_value2[] = {4};
    bitwise_integer<64> n2(init_value2);

    const auto n = n1 / n2;
    BOOST_TEST(n1.ref_chunks()[0] == 11);
    BOOST_TEST(n2.ref_chunks()[0] == 4);
    BOOST_TEST(n.ref_chunks()[0] == 2);
}

BOOST_AUTO_TEST_CASE(mod)
{
    const uint64_t init_value1[] = {11};
    bitwise_integer<64> n1(init_value1);

    const uint64_t init_value2[] = {4};
    bitwise_integer<64> n2(init_value2);

    n1 %= n2;
    BOOST_TEST(n1.ref_chunks()[0] == 3);
    BOOST_TEST(n2.ref_chunks()[0] == 4);
}

BOOST_AUTO_TEST_CASE(mod_binary_opreator)
{
    const uint64_t init_value1[] = {11};
    bitwise_integer<64> n1(init_value1);

    const uint64_t init_value2[] = {4};
    bitwise_integer<64> n2(init_value2);

    const auto n = n1 % n2;
    BOOST_TEST(n1.ref_chunks()[0] == 11);
    BOOST_TEST(n2.ref_chunks()[0] == 4);
    BOOST_TEST(n.ref_chunks()[0] == 3);
}

BOOST_AUTO_TEST_SUITE_END()
