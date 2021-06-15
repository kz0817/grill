#pragma once
#include <memory>
#include <type_traits>
#include <cstddef>
#include <cassert>

namespace grill {

template<typename T>
class block_allocator {
public:
    block_allocator() {
        this->cache_tables = new block_packet*[this->cache_table_size];
        for (std::size_t i = 0; i < this->cache_table_size; i++)
            this->cache_tables[i] = nullptr;
    }

    virtual ~block_allocator() {
        // TODO: free cache_table
    }

    T* take(const std::size_t num_blocks) {
        block_packet* head = get_list_head(num_blocks);
        if (head == nullptr)
            return create_block_packet(num_blocks)->blocks;

        set_list_head(num_blocks, head->next);
        return head->blocks;
    }

    void free(T* blocks) {
        block_packet* pkt = calc_object_addr(blocks);

        // TODO: add pkt deletion when the number of caches is over the upper limit.

        block_packet* head = get_list_head(pkt->num_blocks);
        pkt->next = head;
        set_list_head(pkt->num_blocks, pkt);
    }

private:
    struct block_packet {
        block_packet *next;
        std::size_t num_blocks;
        T blocks[];
    };
    static_assert(std::is_standard_layout<block_packet>::value,
                  "block_packet must be standard layout");

    static constexpr std::size_t DEFAULT_TABLE_SIZE = 256; // 16384 (8192*2) bit
    static constexpr std::size_t OffsetBlocks = offsetof(block_packet, blocks);
    block_packet** cache_tables = nullptr;
    std::size_t cache_table_size = DEFAULT_TABLE_SIZE;

    block_packet* get_list_head(const std::size_t num_blocks) {
        const std::size_t idx = num_blocks - 1;
        if (idx >= this->cache_table_size)
            return nullptr;
        return this->cache_tables[idx];
    }

    void set_list_head(const std::size_t num_blocks, block_packet* pkt) {
        const std::size_t idx = num_blocks - 1;
        if (idx >= this->cache_table_size)
            resize_cache_table(num_blocks);

        this->cache_tables[idx] = pkt;
    }

    void resize_cache_table(const std::size_t num_tables) {
        // TODO: implement
        assert(false);
    }

    block_packet* create_block_packet(const std::size_t num_blocks) {
        const std::size_t pkt_size = sizeof(block_packet) + sizeof(T) * num_blocks;
        block_packet* pkt = reinterpret_cast<block_packet *>(new uint8_t[pkt_size]);
        pkt->next = nullptr;
        pkt->num_blocks = num_blocks;
        return pkt;
    }

    block_packet* calc_object_addr(T* blocks) {
        uint8_t *addr = reinterpret_cast<uint8_t*>(blocks);
        return reinterpret_cast<block_packet *>(addr - OffsetBlocks);
    }
};

} // namespace grill
