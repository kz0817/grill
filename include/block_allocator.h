#pragma once
#include <memory>
#include "integer.h"

namespace grill {

class block_allocator {
public:
    block_allocator();
    virtual ~block_allocator();

    integer::block_t* take(const std::size_t num_blocks);
    void free(integer::block_t* blocks);

private:
    struct private_context;
    std::unique_ptr<private_context> ctx;
};

} // namespace grill
