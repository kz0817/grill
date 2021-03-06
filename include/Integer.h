#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <cassert>
#include "BlockAllocator.h"
#include "gear.h"

namespace grill {

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
        gear::copy(this->blocks, n.blocks, this->num_blocks);
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
     * @param src An initial value. The most significant block first.
     */
    Integer(const std::initializer_list<block_t>& src)
    : Integer(src.size()) {
        int idx = this->num_blocks - 1;
        for (const block_t& v: src)
            blocks[idx--] = v;
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

    /**
     * Adds the given Integer value to this.
     *
     * @param n A right-hand side Integer value.
     * @return This Integer value which is a sum result.
     */
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

    /**
     * Adds the given Inger value and this.
     *
     * @param r A right-hand side Integer value.
     * @return The sum.
     */
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

    /**
     * Set a bit value at the specified position.
     *
     * @param b The bit position to set. The LSB bit is 0.
     * @param v true or false.
     * @return This instance after the bit is set.
     */
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
     * Calculates the inverse number.
     *
     * @param mod A modulo number.
     * @return The inverse number modulo mod.
     */
    Integer inverse(const Integer& mod) const;

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
        return gear::is_all_zero(this->blocks, this->num_blocks);
    }

protected:
    /**
     * Constructor
     *
     * The number of blocks of the created Integer is n_blk.
     * The value of the created Integer is undefined.
     *
     * @param n_blk The number of blocks the created Integer instance has.
     */
    Integer(const std::size_t n_blk)
    : num_blocks(n_blk),
      blocks(get_allocator()->take(this->num_blocks)) {
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

} // namespace grill
