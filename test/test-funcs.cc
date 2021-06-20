#include "test-funcs.h"

using namespace grill;

std::vector<Integer::block_t> create_block_vector(const Integer& n) {
    const int num_blocks = n.get_num_blocks();
    std::vector<Integer::block_t> vec;
    vec.reserve(num_blocks);
    const Integer::block_t* blocks = n.ref_blocks();
    for (int i = 0; i < num_blocks; i++)
        vec.emplace_back(blocks[i]);
    return vec;
}
