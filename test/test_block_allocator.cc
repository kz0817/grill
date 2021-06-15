#include <boost/test/unit_test.hpp>
#include "block_allocator.h"
#include "integer.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_block_allocator)

BOOST_AUTO_TEST_CASE(simple_usage)
{
    block_allocator<integer::block_t> allocator;
    integer::block_t *blocks1 = allocator.take(1);
    BOOST_TEST(blocks1 != nullptr);
    allocator.free(blocks1);

    integer::block_t *blocks2 = allocator.take(1);
    BOOST_TEST(blocks1 == blocks2);
    allocator.free(blocks2);
}

BOOST_AUTO_TEST_CASE(create_two_object)
{
    block_allocator<integer::block_t> allocator;
    integer::block_t *blocks1 = allocator.take(1);
    BOOST_TEST(blocks1 != nullptr);

    integer::block_t *blocks2 = allocator.take(1);
    BOOST_TEST(blocks1 != blocks2);
    allocator.free(blocks1);
    allocator.free(blocks2);

    integer::block_t *blocks3 = allocator.take(1);
    BOOST_TEST(blocks3 == blocks2);
    allocator.free(blocks3);
}

BOOST_AUTO_TEST_CASE(multiple_size)
{
    block_allocator<integer::block_t> allocator;
    integer::block_t *blocks1 = allocator.take(1);
    BOOST_TEST(blocks1 != nullptr);
    allocator.free(blocks1);

    integer::block_t *blocks2 = allocator.take(4);
    BOOST_TEST(blocks1 != blocks2);
    allocator.free(blocks2);
}

BOOST_AUTO_TEST_SUITE_END()
