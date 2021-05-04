#include <boost/test/unit_test.hpp>
#include <vector>
#include "integer.h"

static std::vector<integer::block_t> create_block_vector(const integer& n) {
    const int num_blocks = n.get_num_blocks();
    std::vector<integer::block_t> vec;
    vec.reserve(num_blocks);
    const integer::block_t* blocks = n.ref_blocks();
    for (int i = 0; i < num_blocks; i++)
        vec.emplace_back(blocks[i]);
    return vec;
}

BOOST_AUTO_TEST_SUITE(test_suite_integer)

BOOST_AUTO_TEST_CASE(get_bytes)
{
    wide_int<64> n;
    BOOST_TEST(n.get_num_blocks() == 1);
}

BOOST_AUTO_TEST_CASE(initial_value)
{
    wide_int<64> n;
    BOOST_TEST(n.ref_blocks()[0] == 0);
    BOOST_TEST(static_cast<std::string>(n) == "0");
}

BOOST_AUTO_TEST_CASE(one_initial_value)
{
    wide_int<64> n(5);
    BOOST_TEST(n.ref_blocks()[0] == 5);
    BOOST_TEST(static_cast<std::string>(n) == "5");
}

BOOST_AUTO_TEST_CASE(given_initial_value)
{
    const uint64_t init_value[] = {123};
    wide_int<64> n(init_value);
    BOOST_TEST(n.ref_blocks()[0] == 123);
    BOOST_TEST(static_cast<std::string>(n) == "123");
}

BOOST_AUTO_TEST_CASE(given_initial_value_many_blocks)
{
    const uint64_t init_value[] = {1, 2, 3, 4};
    wide_int<256> n(init_value);
    const integer::block_t expected[] = {4, 3, 2, 1};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(given_initial_value_with_initializer_list)
{
    wide_int<64> n({123});
    BOOST_TEST(n.ref_blocks()[0] == 123);
    BOOST_TEST(static_cast<std::string>(n) == "123");
}

BOOST_AUTO_TEST_CASE(given_initial_value_with_initializer_list_with_mutiple_data)
{
    wide_int<256> n({1, 2, 3});
    const integer::block_t expected[] = {3, 2, 1, 0};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(given_initial_value_with_initializer_list_with_full_data)
{
    wide_int<256> n({1, 2, 3, 4});
    const integer::block_t expected[] = {4, 3, 2, 1};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    wide_int<64> n1 = 123;
    const auto n2(n1);

    BOOST_TEST(n2.ref_blocks()[0] == 123);
    BOOST_TEST(static_cast<std::string>(n2) == "123");
    BOOST_TEST(n1.is_blocks_owner() == false);
    BOOST_TEST(n2.is_blocks_owner() == true);
    BOOST_TEST(n1.ref_blocks() != n2.ref_blocks());
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    wide_int<64> n1 = 123;
    const auto n2(std::move(n1));

    BOOST_TEST(n2.ref_blocks()[0] == 123);
    BOOST_TEST(static_cast<std::string>(n2) == "123");
    BOOST_TEST(n1.is_blocks_owner() == false);
    BOOST_TEST(n2.is_blocks_owner() == false);
    BOOST_TEST(n1.ref_blocks() == n2.ref_blocks());
}

BOOST_AUTO_TEST_CASE(add)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    n1 += n2;
    BOOST_TEST(n1.ref_blocks()[0] == 13);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(add_binary_operator)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    const auto n = n1 + n2;
    BOOST_TEST(n1.ref_blocks()[0] == 10);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
    BOOST_TEST(n.ref_blocks()[0] == 13);
    BOOST_TEST(n.is_blocks_owner() == true);
}

BOOST_AUTO_TEST_CASE(sub)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    n1 -= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 7);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(sub_binary_opreator)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    const auto n = n1 - n2;
    BOOST_TEST(n1.ref_blocks()[0] == 10);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
    BOOST_TEST(n.ref_blocks()[0] == 7);
    BOOST_TEST(n.is_blocks_owner() == true);
}

BOOST_AUTO_TEST_CASE(mul)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    n1 *= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 30);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
}

BOOST_AUTO_TEST_CASE(mul_binary_opreator)
{
    wide_int<64> n1 = 10;
    wide_int<64> n2 = 3;

    const auto n = n1 * n2;
    BOOST_TEST(n1.ref_blocks()[0] == 10);
    BOOST_TEST(n2.ref_blocks()[0] == 3);
    BOOST_TEST(n.ref_blocks()[0] == 30);
    BOOST_TEST(n.is_blocks_owner() == true);
}

BOOST_AUTO_TEST_CASE(div)
{
    wide_int<64> n1 = 11;
    wide_int<64> n2 = 4;

    n1 /= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 2);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
}

BOOST_AUTO_TEST_CASE(div_binary_opreator)
{
    wide_int<64> n1 = 11;
    wide_int<64> n2 = 4;

    const auto n = n1 / n2;
    BOOST_TEST(n1.ref_blocks()[0] == 11);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
    BOOST_TEST(n.ref_blocks()[0] == 2);
    BOOST_TEST(n.is_blocks_owner() == true);
}

BOOST_AUTO_TEST_CASE(mod)
{
    wide_int<64> n1 = 11;
    wide_int<64> n2 = 4;

    n1 %= n2;
    BOOST_TEST(n1.ref_blocks()[0] == 3);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
}

BOOST_AUTO_TEST_CASE(mod_binary_opreator)
{
    wide_int<64> n1 = 11;
    wide_int<64> n2 = 4;

    const auto n = n1 % n2;
    BOOST_TEST(n1.ref_blocks()[0] == 11);
    BOOST_TEST(n2.ref_blocks()[0] == 4);
    BOOST_TEST(n.ref_blocks()[0] == 3);
    BOOST_TEST(n.is_blocks_owner() == true);
}

BOOST_AUTO_TEST_CASE(substitution)
{
    wide_int<256> n({1, 2, 3, 4});
    n = 5;

    const integer::block_t expected[] = {5, 0, 0, 0};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(pow)
{
    const wide_int<64> n1 = 3;
    const wide_int<64> e = 4;
    const auto n2 = n1.pow(e);
    BOOST_TEST(n2.ref_blocks()[0] == 81);
}

BOOST_AUTO_TEST_SUITE_END()
