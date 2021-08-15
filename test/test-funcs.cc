#include <cstdlib>
#include <string>
#include "test-funcs.h"

using namespace grill;

static const char* const ENV_VERBOSE_NAME = "VERBOSE";

std::vector<Integer::block_t> create_block_vector(const Integer& n) {
    const int num_blocks = n.get_num_blocks();
    std::vector<Integer::block_t> vec;
    vec.reserve(num_blocks);
    const Integer::block_t* blocks = n.ref_blocks();
    for (int i = 0; i < num_blocks; i++)
        vec.emplace_back(blocks[i]);
    return vec;
}

bool is_verbose() {
    const char *var = getenv(ENV_VERBOSE_NAME);
    if (var == nullptr)
        return false;
    return (std::string(var) == "1");
}
