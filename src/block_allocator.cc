#include <type_traits>
#include <cstddef>
#include <map>
#include "block_allocator.h"
#include "integer.h"

namespace grill {

struct block_packet {
    block_packet *next;
    std::size_t num_blocks;
    integer::block_t blocks[];
};
static_assert(std::is_standard_layout<block_packet>::value, "block_packet must be standard layout");

static constexpr std::size_t OffsetBlocks = offsetof(block_packet, blocks);

struct block_allocator::private_context {
    std::map<int, block_packet*> cache_table;

    block_packet* get_list_head(const std::size_t num_blocks) {
        const auto it = this->cache_table.find(num_blocks);
        if (it != this->cache_table.end())
            return it->second;
        return nullptr;
    }

    void set_list_head(const std::size_t num_blocks, block_packet* pkt) {
        this->cache_table[num_blocks] = pkt;
    }

    block_packet* create_block_packet(const std::size_t num_blocks) {
        const std::size_t pkt_size = sizeof(block_packet) + sizeof(integer::block_t) * num_blocks;
        block_packet* pkt = reinterpret_cast<block_packet *>(new uint8_t[pkt_size]);
        pkt->next = nullptr;
        pkt->num_blocks = num_blocks;
        return pkt;
    }

    block_packet* calc_object_addr(integer::block_t* blocks) {
        uint8_t *addr = reinterpret_cast<uint8_t*>(blocks);
        return reinterpret_cast<block_packet *>(addr - OffsetBlocks);
    }

    integer::block_t* take(const std::size_t num_blocks) {
        block_packet* head = get_list_head(num_blocks);
        if (head == nullptr)
            return create_block_packet(num_blocks)->blocks;

        set_list_head(num_blocks, head->next);
        return head->blocks;
    }

    void free(integer::block_t* blocks) {
        block_packet* pkt = calc_object_addr(blocks);

        // TODO: add pkt deletion when the number of caches is over the upper limit.

        block_packet* head = get_list_head(pkt->num_blocks);
        pkt->next = head;
        set_list_head(pkt->num_blocks, pkt);
    }
};

block_allocator::block_allocator()
: ctx(new private_context()) {
}

block_allocator::~block_allocator() {
}

integer::block_t* block_allocator::take(const std::size_t num_blocks) {
    return ctx->take(num_blocks);
}

void block_allocator::free(integer::block_t* blocks) {
    ctx->free(blocks);
}

} // namespace grill
