#pragma once
#include <memory>
#include <type_traits>
#include <cstddef>
#include <cassert>

namespace grill {

template<typename T>
class BlockAllocator {
public:
    BlockAllocator() {
        this->cache_tables = new BlockPacket*[this->cache_table_size];
        for (std::size_t i = 0; i < this->cache_table_size; i++)
            this->cache_tables[i] = nullptr;
    }

    virtual ~BlockAllocator() {
        // TODO: free cache_table
    }

    T* take(const std::size_t num_blocks) {
        BlockPacket* head = get_list_head(num_blocks);
        if (head == nullptr)
            return create_BlockPacket(num_blocks)->blocks;

        set_list_head(num_blocks, head->next);
        return head->blocks;
    }

    void free(T* blocks) {
        BlockPacket* pkt = calc_object_addr(blocks);

        // TODO: add pkt deletion when the number of caches is over the upper limit.

        BlockPacket* head = get_list_head(pkt->num_blocks);
        pkt->next = head;
        set_list_head(pkt->num_blocks, pkt);
    }

private:
    struct BlockPacket {
        BlockPacket *next;
        std::size_t num_blocks;
        T blocks[];
    };
    static_assert(std::is_standard_layout<BlockPacket>::value,
                  "BlockPacket must be standard layout");

    static constexpr std::size_t DEFAULT_TABLE_SIZE = 256; // 16384 (8192*2) bit
    static constexpr std::size_t OffsetBlocks = offsetof(BlockPacket, blocks);
    BlockPacket** cache_tables = nullptr;
    std::size_t cache_table_size = DEFAULT_TABLE_SIZE;

    BlockPacket* get_list_head(const std::size_t num_blocks) {
        const std::size_t idx = num_blocks - 1;
        if (idx >= this->cache_table_size)
            return nullptr;
        return this->cache_tables[idx];
    }

    void set_list_head(const std::size_t num_blocks, BlockPacket* pkt) {
        const std::size_t idx = num_blocks - 1;
        if (idx >= this->cache_table_size)
            resize_cache_table(num_blocks);

        this->cache_tables[idx] = pkt;
    }

    void resize_cache_table(const std::size_t num_tables) {
        // TODO: implement
        assert(false);
    }

    BlockPacket* create_BlockPacket(const std::size_t num_blocks) {
        const std::size_t pkt_size = sizeof(BlockPacket) + sizeof(T) * num_blocks;
        BlockPacket* pkt = reinterpret_cast<BlockPacket *>(new uint8_t[pkt_size]);
        pkt->next = nullptr;
        pkt->num_blocks = num_blocks;
        return pkt;
    }

    BlockPacket* calc_object_addr(T* blocks) {
        uint8_t *addr = reinterpret_cast<uint8_t*>(blocks);
        return reinterpret_cast<BlockPacket *>(addr - OffsetBlocks);
    }
};

} // namespace grill
