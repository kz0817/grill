#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include "Integer.h"
#include "test-funcs.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_Integer_ctor_dtor)

BOOST_AUTO_TEST_CASE(default_constructor)
{
    const Integer n({0}); // TODO: n;
    BOOST_TEST(n.ref_blocks()[0] == 0);
    BOOST_TEST(n.get_num_blocks() == 1);
}

BOOST_AUTO_TEST_CASE(one_initial_value)
{
    const Integer n({5});
    BOOST_TEST(n.ref_blocks()[0] == 5);
}

BOOST_AUTO_TEST_CASE(given_initial_value_with_initializer_list)
{
    const Integer n({123});
    BOOST_TEST(n.ref_blocks()[0] == 123);
}

BOOST_AUTO_TEST_CASE(given_initial_value_with_initializer_list_with_full_data)
{
    const Integer n({1, 2, 3, 4});
    const Integer::block_t expected[] = {4, 3, 2, 1};
    BOOST_TEST(create_block_vector(n) == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    const Integer n1({123});
    const auto n2(n1);

    BOOST_TEST(n2.ref_blocks()[0] == 123);
    BOOST_TEST(n1.ref_blocks() != n2.ref_blocks());
    BOOST_TEST(n1.ref_blocks() != nullptr);
    BOOST_TEST(n2.ref_blocks() != nullptr);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    Integer n1({123});
    const auto n1_ref_blocks = n1.ref_blocks();
    const auto n2(std::move(n1));

    BOOST_TEST(n2.ref_blocks()[0] == 123);
    BOOST_TEST(n1_ref_blocks == n2.ref_blocks());
    BOOST_TEST(n1.ref_blocks() == nullptr);
}

BOOST_AUTO_TEST_SUITE_END()
