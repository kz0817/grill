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

    std::size_t get_num_blocks() const;
    const block_t* ref_blocks() const;

    operator std::string() const;
    friend std::ostream& operator<<(std::ostream& os, const integer& data);

    integer& operator+=(const integer& n);
    integer& operator-=(const integer& n);
    integer& operator*=(const integer& n);
    integer& operator/=(const integer& n);
    integer& operator%=(const integer& n);

    integer& operator=(integer&& n);

    integer operator+(const integer& r) const;
    integer operator-(const integer& r) const;
    integer operator*(const integer& r) const;
    integer operator/(const integer& r) const;
    integer operator%(const integer& r) const;

    bool operator==(const integer& r) const;
    bool operator!=(const integer& r) const;
    bool operator>=(const integer& r) const;
    bool operator<=(const integer& r) const;

    integer operator&(const integer& r) const;

    integer& operator<<=(const unsigned int r);
    integer& operator>>=(const unsigned int r);
    integer& operator++();

    int most_significant_active_bit() const;
    bool get_bit_value(const int b) const;
    integer& set_bit_value(const int b, const bool v);
    integer pow(const integer& e) const;
    integer pow_mod(const integer& e, const integer& mod) const;

    /**
     * Returns the value of 2 to the power of e.
     *
     * The number of blocks of the returned integer is automatically decided.
     *
     * @param e An exponent.
     * @return The value of 2 to the power of e.
     */
    static integer pow2(const int e);

    bool is_odd() const;
    bool is_even() const;

    /**
     * Returns whether the value is zero.
     *
     * @return true if the value is zero. Otherwise false.
     */
    bool is_zero() const;

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
    static_assert((N % 64) == 0, "N must be a multiple of 64");
    static constexpr std::size_t B = (N / 64);

public:
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
