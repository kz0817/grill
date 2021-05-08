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

    integer(const integer& n);
    integer(integer&& n);
    integer(const std::size_t n_blk, const std::initializer_list<block_t>& src);
    virtual ~integer();
    integer create(const integer::block_t init_value) const;

    std::size_t get_num_blocks() const;
    const block_t* ref_blocks() const;

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

    integer& operator<<=(const unsigned int r);

    integer pow(const integer& e) const;

protected:
    integer(const std::size_t n_blk);
    integer(const std::size_t n_blk, const block_t* const src);
    block_t* get_blocks() const;

private:
    std::size_t num_blocks;
    block_t* blocks; // Least significant block first
};

template<std::size_t N>
class wide_int : public integer {
public:
    static_assert((N % 64) == 0, "N must be a multiple of 64");
    static constexpr std::size_t B = (N / 64);

    wide_int(const integer::block_t init_value = 0)
    : integer(B, {init_value}) {
    }

    wide_int(const std::uint64_t src[B])
    : integer(B, src) {
    }

    wide_int(const std::initializer_list<integer::block_t>& src)
    : integer(B, src) {
    }
};

} // namespace grill
