#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <cstring>
#include <cassert>
#include "BlockAllocator.h"

namespace grill {

class Integer {
public:
    using block_t = uint64_t;
    static constexpr int BlockBits = sizeof(Integer::block_t) * 8;

    Integer(const Integer& n);
    Integer(Integer&& n);
    Integer(const std::size_t n_blk, const std::initializer_list<block_t>& src);

    /**
     * Destructor
     */
    virtual ~Integer() {
        if (this->blocks != nullptr)
            this->allocator->free(this->blocks);
    }

    std::size_t get_num_blocks() const;
    const block_t* ref_blocks() const;

    operator std::string() const;
    friend std::ostream& operator<<(std::ostream& os, const Integer& data);

    Integer& operator+=(const Integer& n);
    Integer& operator-=(const Integer& n);
    Integer& operator*=(const Integer& n);
    Integer& operator/=(const Integer& n);
    Integer& operator%=(const Integer& n);

    Integer& operator=(Integer&& n);

    Integer operator+(const Integer& r) const;
    Integer operator-(const Integer& r) const;
    Integer operator*(const Integer& r) const;
    Integer operator/(const Integer& r) const;
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
    static Integer pow2(const int e);

    bool is_odd() const;
    bool is_even() const;

    /**
     * Returns whether the value is zero.
     *
     * @return true if the value is zero. Otherwise false.
     */
    bool is_zero() const;

protected:
    Integer(const std::size_t n_blk);

    /**
     * Constructor
     *
     * The number of blocks of the create Integer is n_blk.
     *
     * @param n_blk The number of src.
     * @param src An initial value array for this instance. The most siginificant block first.
     */
    Integer(const std::size_t n_blk, const block_t* const src);

    block_t* get_blocks() const;

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
