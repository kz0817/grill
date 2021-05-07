#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <cstring>
#include <cassert>

namespace grill {

class integer {
public:
    using block_t = uint64_t;

    integer(const integer& n, const bool skip_blocks_copy=false);
    integer(integer&& n);
    integer(const std::size_t num, block_t* const buf);
    virtual ~integer();
    integer create(const integer::block_t init_value) const;

    std::size_t get_num_blocks() const;
    const block_t* ref_blocks() const;
    bool is_blocks_owner() const;

    operator std::string() const;
    friend std::ostream& operator<<(std::ostream& os, const integer& data);

    integer& operator+=(const integer& n);
    integer& operator-=(const integer& n);
    integer& operator*=(const integer& n);
    integer& operator/=(const integer& n);
    integer& operator%=(const integer& n);
    integer& operator=(const block_t n);
    integer& operator=(integer&& n);

    integer operator+(const integer& r) const;
    integer operator-(const integer& r) const;
    integer operator*(const integer& r) const;
    integer operator/(const integer& r) const;
    integer operator%(const integer& r) const;

    bool operator==(const integer& r) const;
    bool operator!=(const integer& r) const;
    bool operator>=(const integer& r) const;

    integer operator&(const integer& r) const;

    integer& operator<<=(const int r);

    integer pow(const integer& e) const;

protected:
    block_t* get_blocks() const;

private:
    std::size_t num_blocks;
    block_t* blocks; // Least significant block first
    bool blocks_owner;
};

template<std::size_t N>
class scalable_int : public integer {
public:
    scalable_int(const block_t init_value = 0)
    : scalable_int({init_value}) {
    }

    scalable_int(const block_t src[N])
    : integer(N, blocks) {
        for (std::size_t i = 0; i < N; i++)
            blocks[i] = src[N-i-1];
    }

    scalable_int(const std::initializer_list<block_t>& src)
    : integer(N, blocks) {
        const std::size_t num_args = src.size();
        assert(num_args <= N);
        int idx = num_args - 1;
        for (const block_t& v: src)
            blocks[idx--] = v;

        const std::size_t num_zero_blocks = N - num_args;
        if (num_zero_blocks > 0)
            memset(&blocks[num_args], 0, sizeof(block_t) * num_zero_blocks);
    }

private:
    block_t blocks[N];
};

template<std::size_t N>
class wide_int : public scalable_int<N/64> {
public:
    wide_int(const integer::block_t init_value = 0)
    : scalable_int<N/64>(init_value) {
    }

    wide_int(const std::uint64_t src[N/64])
    : scalable_int<N/64>(src) {
    }

    wide_int(const std::initializer_list<integer::block_t>& src)
    : scalable_int<N/64>(src) {
    }

    integer& operator=(const integer::block_t n) {
        return static_cast<integer&>(*this) = n;
    }
};

} // namespace grill
