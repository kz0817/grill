#include <type_traits>
#include <cstddef>
#include <unordered_map>
#include "block_allocator.h"
#include "integer.h"

namespace grill {

struct block_packet {
    std::size_t num_blocks;
    block_packet *next;
    integer::block_t blocks[];
};
static_assert(std::is_standard_layout<block_packet>::value, "block_packet must be standard layout");

struct block_packet_list_head {
    block_packet *next = nullptr;
};

static constexpr std::size_t OffsetBlocks = offsetof(block_packet, blocks);

struct block_allocator::private_context {
    std::unordered_map<std::size_t, block_packet_list_head *> cache_table;

    block_packet_list_head* get_list_head(const std::size_t num_blocks) {
        const auto it = this->cache_table.find(num_blocks);
        if (it != this->cache_table.end())
            return it->second;

        block_packet_list_head* head = new block_packet_list_head;
        this->cache_table[num_blocks] = head;
        return head;
    }

    block_packet* create_block_packet(const std::size_t num_blocks) {
        const std::size_t pkt_size = sizeof(block_packet) + sizeof(integer::block_t) * num_blocks;
        block_packet* pkt = reinterpret_cast<block_packet *>(new uint8_t[pkt_size]);
        pkt->num_blocks = num_blocks;
        pkt->next = nullptr;
        return pkt;
    }

    block_packet* calc_object_addr(integer::block_t* blocks) {
        uint8_t *addr = reinterpret_cast<uint8_t*>(blocks);
        return reinterpret_cast<block_packet *>(addr - OffsetBlocks);
    }

    integer::block_t* take(const std::size_t num_blocks) {
        block_packet_list_head* head = get_list_head(num_blocks);
        block_packet *pkt = head->next;
        if (pkt != nullptr)
            head->next = pkt->next; // take the first entry out
        else
            pkt = create_block_packet(num_blocks);
        return pkt->blocks;
    }

    void free(integer::block_t* blocks) {
        block_packet* pkt = calc_object_addr(blocks);

        // TODO: add pkt deletion when the number of caches is over the upper limit.

        block_packet_list_head* head = get_list_head(pkt->num_blocks);
        pkt->next = head->next;
        head->next = pkt;
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
