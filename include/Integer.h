#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <cassert>
#include "BlockAllocator.h"

namespace grill {

namespace internal_impl {
template<typename T>
bool is_all_zero(const T* blocks, const std::size_t n) {
    for (std::size_t i = 0; i < n; i++) {
        if (blocks[i] != 0)
            return false;
    }
    return true;
}

template<typename T>
void copy(T* dest, const T* src, std::size_t n) {
    for (std::size_t i = 0; i < n; i++)
        dest[i] = src[i];
}

template<typename T>
void fill_zero(T* dest, std::size_t n) {
    for (std::size_t i = 0; i < n; i++)
        dest[i] = 0;
}

} // namespace internal_impl

class Integer {
public:
    using block_t = uint64_t;
    static constexpr int BlockBits = sizeof(Integer::block_t) * 8;

    /**
     * Copy constructor
     *
     * @param n An Integer instance.
     */
    Integer(const Integer& n)
    : Integer(n.num_blocks) {
        internal_impl::copy(this->blocks, n.blocks, this->num_blocks);
    }

    /**
     * Move constructor
     *
     * @param n An Integer instance.
     */
    Integer(Integer&& n)
    : num_blocks(n.num_blocks),
      blocks(n.blocks) {
        n.blocks = nullptr;
    }

    /**
     * Constructor with the initial value.
     *
     * The number of blocks of the created Integer is n_blk.
     *
     * @param n_blk The number of src.
     * @param src An initial value. The most siginificant block first.
     */
    Integer(const std::size_t n_blk, const std::initializer_list<block_t>& src)
    : Integer(n_blk) {
        const std::size_t num_args = src.size();
        assert(num_args <= this->num_blocks);

        int idx = num_args - 1;
        for (const block_t& v: src)
            blocks[idx--] = v;

        const std::size_t num_zero_blocks = this->num_blocks - num_args;
        if (num_zero_blocks > 0)
            internal_impl::fill_zero(&blocks[num_args], num_zero_blocks);
    }

    /**
     * Copy constructor with the given number of blocks.
     *
     * If the number of blocks of the source Integer is less than that of n_blk,
     * the lower n_blk blocks are copied.
     *
     * @param n_blk The number of blocks of the Integer to create.
     * @param src A source Integer.
     */
    Integer(const std::size_t n_blk, const Integer& src)
    : Integer(n_blk) {
        const std::size_t n_src_blk = src.get_num_blocks();

        const std::size_t num_copy_blocks = (n_blk >= n_src_blk) ? n_src_blk : n_blk;
        internal_impl::copy(this->blocks, src.blocks, num_copy_blocks);

        const std::size_t num_zero_blocks = this->num_blocks - num_copy_blocks;
        internal_impl::fill_zero(&blocks[num_copy_blocks], num_zero_blocks);
    }

    /**
     * Destructor
     */
    virtual ~Integer() {
        if (this->blocks != nullptr)
            this->allocator->free(this->blocks);
    }

    /**
     * Returns the number of internal blocks.
     *
     * @return The number of internal blocks.
     */
    std::size_t get_num_blocks() const {
        return this->num_blocks;
    }

    /**
     * Returns the internal blocks.
     *
     * @return The const pointer of internal blocks.
     */
    const block_t* ref_blocks() const {
        return get_blocks();
    }

    operator std::string() const;
    friend std::ostream& operator<<(std::ostream& os, const Integer& data);

    Integer& operator+=(const Integer& n);

    /**
     * Subtracts the given Integer value from this.
     *
     * @param r A right-hand side Integer value.
     * @return This Integer value which is a subtraction result.
     */
    Integer& operator-=(const Integer& n);

    Integer& operator*=(const Integer& n);

    /**
     * Divide this Integer by the givin Integer.
     *
     * @param r A right-hand side Integer value.
     * @return This Integer which is a quotient of the division.
     */
    Integer& operator/=(const Integer& n);

    /**
     * Calculates the quotient of division.
     *
     * @param r A right-hand side Integer value.
     * @return This Integer which is a quotient of the division.
     */
    Integer& operator%=(const Integer& n);

    Integer& operator=(Integer&& n);

    Integer operator+(const Integer& r) const;

    /**
     * Subtracts the given Integer value from this.
     *
     * The digits of the result is the same as this Integer (left-hand side).
     *
     * @param r A right-hand side Integer value.
     * @return The subtraction result as a newly created Integer.
     */
    Integer operator-(const Integer& r) const;

    Integer operator*(const Integer& r) const;

    /**
     * Calculates the quotient of division.
     *
     * The digits of the result is the same as this Integer (left-hand side).
     *
     * @param r A right-hand side Integer value.
     * @return The quotient.
     */
    Integer operator/(const Integer& r) const;

    /**
     * Calculates the remainder of division.
     *
     * The digits of the result is the same as this Integer (left-hand side).
     *
     * @param r A right-hand side Integer value.
     * @return The remainder.
     */
    Integer operator%(const Integer& r) const;

    bool operator==(const Integer& r) const;
    bool operator!=(const Integer& r) const;
    bool operator>=(const Integer& r) const;
    bool operator<=(const Integer& r) const;

    Integer operator&(const Integer& r) const;

    Integer& operator<<=(const unsigned int r);
    Integer& operator>>=(const unsigned int r);
    Integer& operator++();

    int most_significant_active_bit() const;
    bool get_bit_value(const int b) const;
    Integer& set_bit_value(const int b, const bool v);
    Integer pow(const Integer& e) const;
    Integer pow_mod(const Integer& e, const Integer& mod) const;

    /**
     * Returns the value of 2 to the power of e.
     *
     * The number of blocks of the returned Integer is automatically decided.
     *
     * @param e An exponent.
     * @return The value of 2 to the power of e.
     */
    static const Integer& pow2(const int e);

    /**
     * Returns whether the value is odd.
     *
     * @return true if the value is odd. Otherwise false.
     */
    bool is_odd() const {
        return this->blocks[0] & 1;
    }

    /**
     * Returns whether the value is even.
     *
     * @return true if the value is even. Otherwise false.
     */
    bool is_even() const {
        return !is_odd();
    }

    /**
     * Returns whether the value is zero.
     *
     * @return true if the value is zero. Otherwise false.
     */
    bool is_zero() const {
        return internal_impl::is_all_zero(this->blocks, this->num_blocks);
    }

protected:
    /**
     * Constructor
     *
     * The number of blocks of the created Integer is n_blk.
     * The value of the created Integer is undefined.
     *
     * @param n_blk The number of blocks the craeted Integer instance has.
     */
    Integer(const std::size_t n_blk)
    : num_blocks(n_blk),
      blocks(get_allocator()->take(this->num_blocks)) {
    }

    /**
     * Constructor with the initial value.
     *
     * The number of blocks of the created Integer is n_blk.
     *
     * @param n_blk The number of src.
     * @param src An initial value array. The most siginificant block first.
     */
    Integer(const std::size_t n_blk, const block_t* const src)
    : Integer(n_blk) {
        for (std::size_t i = 0; i < n_blk; i++) {
            int idx = n_blk - i - 1;
            this->blocks[idx] = src[i];
        }
    }

    /**
     * Returns the internal blocks.
     *
     * @return The pointer of internal blocks.
     */
    block_t* get_blocks() const {
        return this->blocks;
    }

private:
    // C++11's local_thread has a run-time penalty. So we use the GCC's __thread feature.
    // See also: https://gcc.gnu.org/gcc-4.8/changes.html#cxx
    static __thread BlockAllocator<block_t> *allocator;

    std::size_t num_blocks;
    block_t* blocks; // Least significant block first

    inline BlockAllocator<block_t> *get_allocator() {
        // Don't delete 'allocator' because allocator::free() may be called after its destruction
        // due to the destructor of static Integer objects.
        if (this->allocator == nullptr)
            this->allocator = new BlockAllocator<block_t>;
        return allocator;
    }
};

template<std::size_t N>
class WideInt : public Integer {
    static_assert((N % Integer::BlockBits) == 0, "N must be a multiple of Integer::BlockBits");
    static constexpr std::size_t B = (N / Integer::BlockBits);

public:
    WideInt(const Integer::block_t init_value)
    : Integer(B, {init_value}) {
    }

    WideInt(const std::initializer_list<Integer::block_t>& src)
    : Integer(B, src) {
    }
};

} // namespace grill
