#include <boost/test/unit_test.hpp>
#include "BlockAllocator.h"
#include "Integer.h"

using namespace grill;

BOOST_AUTO_TEST_SUITE(test_suite_BlockAllocator)

BOOST_AUTO_TEST_CASE(simple_usage)
{
    BlockAllocator<Integer::block_t> allocator;
    Integer::block_t *blocks1 = allocator.take(1);
    BOOST_TEST(blocks1 != nullptr);
    allocator.free(blocks1);

    Integer::block_t *blocks2 = allocator.take(1);
    BOOST_TEST(blocks1 == blocks2);
    allocator.free(blocks2);
}

BOOST_AUTO_TEST_CASE(create_two_object)
{
    BlockAllocator<Integer::block_t> allocator;
    Integer::block_t *blocks1 = allocator.take(1);
    BOOST_TEST(blocks1 != nullptr);

    Integer::block_t *blocks2 = allocator.take(1);
    BOOST_TEST(blocks1 != blocks2);
    allocator.free(blocks1);
    allocator.free(blocks2);

    Integer::block_t *blocks3 = allocator.take(1);
    BOOST_TEST(blocks3 == blocks2);
    allocator.free(blocks3);
}

BOOST_AUTO_TEST_CASE(multiple_size)
{
    BlockAllocator<Integer::block_t> allocator;
    Integer::block_t *blocks1 = allocator.take(1);
    BOOST_TEST(blocks1 != nullptr);
    allocator.free(blocks1);

    Integer::block_t *blocks2 = allocator.take(4);
    BOOST_TEST(blocks1 != blocks2);
    allocator.free(blocks2);
}

BOOST_AUTO_TEST_SUITE_END()
