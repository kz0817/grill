#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <sstream>
#include "integer.h"
#include "block_allocator.h"
#include "constant.h"

namespace grill {

static constexpr integer::block_t BitMask[] = {
    0x0000'0000'0000'0001, 0x0000'0000'0000'0002, 0x0000'0000'0000'0004, 0x0000'0000'0000'0008,
    0x0000'0000'0000'0010, 0x0000'0000'0000'0020, 0x0000'0000'0000'0040, 0x0000'0000'0000'0080,
    0x0000'0000'0000'0100, 0x0000'0000'0000'0200, 0x0000'0000'0000'0400, 0x0000'0000'0000'0800,
    0x0000'0000'0000'1000, 0x0000'0000'0000'2000, 0x0000'0000'0000'4000, 0x0000'0000'0000'8000,

    0x0000'0000'0001'0000, 0x0000'0000'0002'0000, 0x0000'0000'0004'0000, 0x0000'0000'0008'0000,
    0x0000'0000'0010'0000, 0x0000'0000'0020'0000, 0x0000'0000'0040'0000, 0x0000'0000'0080'0000,
    0x0000'0000'0100'0000, 0x0000'0000'0200'0000, 0x0000'0000'0400'0000, 0x0000'0000'0800'0000,
    0x0000'0000'1000'0000, 0x0000'0000'2000'0000, 0x0000'0000'4000'0000, 0x0000'0000'8000'0000,

    0x0000'0001'0000'0000, 0x0000'0002'0000'0000, 0x0000'0004'0000'0000, 0x0000'0008'0000'0000,
    0x0000'0010'0000'0000, 0x0000'0020'0000'0000, 0x0000'0040'0000'0000, 0x0000'0080'0000'0000,
    0x0000'0100'0000'0000, 0x0000'0200'0000'0000, 0x0000'0400'0000'0000, 0x0000'0800'0000'0000,
    0x0000'1000'0000'0000, 0x0000'2000'0000'0000, 0x0000'4000'0000'0000, 0x0000'8000'0000'0000,

    0x0001'0000'0000'0000, 0x0002'0000'0000'0000, 0x0004'0000'0000'0000, 0x0008'0000'0000'0000,
    0x0010'0000'0000'0000, 0x0020'0000'0000'0000, 0x0040'0000'0000'0000, 0x0080'0000'0000'0000,
    0x0100'0000'0000'0000, 0x0200'0000'0000'0000, 0x0400'0000'0000'0000, 0x0800'0000'0000'0000,
    0x1000'0000'0000'0000, 0x2000'0000'0000'0000, 0x4000'0000'0000'0000, 0x8000'0000'0000'0000,
};

__thread block_allocator<integer::block_t> *integer::allocator = nullptr;

static std::string create_error_msg(const char* const filename, const int lineno,
                                    const char* const msg) {
    std::stringstream ss;
    ss << "[" << filename << ":" << lineno << "] " << msg;
    return ss.str();
}

#define THROW_ERROR(MSG) throw std::runtime_error(create_error_msg(__FILE__, __LINE__, MSG))

static bool is_valid_index(const std::size_t num_blocks, const int idx) {
    if (idx < 0)
        return false;
    return static_cast<std::size_t>(idx) < num_blocks;
}

static integer::block_t upper_half_block(const integer::block_t n) {
    return (n >> (integer::BlockBits/2));
}

static integer::block_t lower_half_block(const integer::block_t n) {
    constexpr integer::block_t mask = ~(-BitMask[integer::BlockBits/2]);
    return mask & n;
}

//
// protected methods
//
integer::integer(const std::size_t n_blk)
: num_blocks(n_blk),
  blocks(get_allocator()->take(this->num_blocks)) {
}

integer::integer(const std::size_t n_blk, const block_t* const src)
: integer(n_blk) {
    for (std::size_t i = 0; i < n_blk; i++) {
        int idx = n_blk - i - 1;
        this->blocks[idx] = src[i];
    }
}

integer::block_t* integer::get_blocks() const {
    return this->blocks;
}

//
// public methods
//
integer::integer(const integer& n)
: integer(n.num_blocks) {
    std::memcpy(this->blocks, n.blocks, sizeof(block_t) * this->num_blocks);
}

integer::integer(integer&& n)
: num_blocks(n.num_blocks),
  blocks(n.blocks) {
    n.blocks = nullptr;
}

integer::integer(const std::size_t n_blk, const std::initializer_list<block_t>& src)
: integer(n_blk) {
    const std::size_t num_args = src.size();
    assert(num_args <= this->num_blocks);

    int idx = num_args - 1;
    for (const block_t& v: src)
        blocks[idx--] = v;

    const std::size_t num_zero_blocks = this->num_blocks - num_args;
    if (num_zero_blocks > 0)
        std::memset(&blocks[num_args], 0, sizeof(block_t) * num_zero_blocks);
}

std::size_t integer::get_num_blocks() const {
    return this->num_blocks;
}

const integer::block_t* integer::ref_blocks() const {
    return get_blocks();
}

integer::operator std::string() const {
    std::stringstream ss;
    const int buf_size = sizeof(block_t) * 2 + 1;
    char buf[buf_size];
    buf[buf_size - 1] = '\0';
    const int num_blocks = get_num_blocks();
    for (int i = 0; i < num_blocks; i++) {
        const int idx = num_blocks - i - 1;
        if (std::snprintf(buf, buf_size, "%016zx", ref_blocks()[idx]) < 0)
            THROW_ERROR("Failed: snprintf()");
        ss << buf;
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const integer& data) {
    os << static_cast<std::string>(data);
    return os;
}

static bool add_one_block(integer::block_t& lhs, const integer::block_t rhs,
                          const bool carry_flag) {
    const integer::block_t prev_lhs = lhs;
    lhs += rhs;
    bool overflow = (lhs < prev_lhs);

    if (carry_flag) {
        lhs++;
        if (lhs == 0)
            overflow = true;
    }
    return overflow;
}

static void add(integer::block_t* dest, const std::size_t num_dest_blocks,
                const integer::block_t* src, const std::size_t num_src_blocks) {
    bool carry_flag = false;
    for (std::size_t i = 0; i < num_dest_blocks; i++) {
        const bool src_is_valid = (i < num_src_blocks);
        if (!carry_flag && !src_is_valid)
            break;
        const integer::block_t src_val = src_is_valid ? src[i] : 0;
        carry_flag = add_one_block(dest[i], src_val, carry_flag);
    }
}

integer& integer::operator+=(const integer& n) {
    return *this = (*this) + n;
}

integer& integer::operator-=(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] -= n.ref_blocks()[0];
    return *this;
}

integer& integer::operator*=(const integer& n) {
    return *this = (*this) * n;
}

integer& integer::operator/=(const integer& n) {
    return *this = (*this) / n;
}

integer& integer::operator%=(const integer& n) {
    return *this = (*this) % n;
}

integer& integer::operator=(integer&& n) {
    get_allocator()->free(this->blocks);

    this->num_blocks = n.num_blocks;
    this->blocks = n.blocks,
    n.blocks = nullptr;

    return *this;
}

integer integer::operator+(const integer& r) const {
    integer n(*this);
    add(n.get_blocks(), n.get_num_blocks(), r.ref_blocks(), r.get_num_blocks());
    return n;
}

integer integer::operator-(const integer& r) const {
    integer n(*this);
    n -= r;
    return n;
}

static void mul_blocks(const integer::block_t lhs, const integer::block_t rhs,
                       integer::block_t dest[2]) {
    const integer::block_t lhs_upper_half = upper_half_block(lhs);
    const integer::block_t lhs_lower_half = lower_half_block(lhs);
    const integer::block_t rhs_upper_half = upper_half_block(rhs);
    const integer::block_t rhs_lower_half = lower_half_block(rhs);
    dest[0] = lhs_lower_half * rhs_lower_half;
    dest[1] = lhs_upper_half * rhs_upper_half;

    const integer::block_t x0 = lhs_upper_half * rhs_lower_half;
    const integer::block_t src0[2] = {x0 << (integer::BlockBits/2), upper_half_block(x0)};
    add(dest, 2, src0, 2);

    const integer::block_t x1 = rhs_upper_half * lhs_lower_half;
    const integer::block_t src1[2] = {x1 << (integer::BlockBits/2), upper_half_block(x1)};
    add(dest, 2, src1, 2);
}

static void mul(const integer& lhs, const integer& rhs,
                integer::block_t* blocks, const std::size_t num_blocks) {
    const std::size_t l_num_blocks = lhs.get_num_blocks();
    const std::size_t r_num_blocks = rhs.get_num_blocks();

    const integer::block_t* l_blocks = lhs.ref_blocks();
    const integer::block_t* r_blocks = rhs.ref_blocks();

    for (std::size_t l_idx = 0; l_idx < l_num_blocks; l_idx++) {
        for (std::size_t r_idx = 0; r_idx < r_num_blocks; r_idx++) {
            // skip calculation for the block beyond the output value range
            const std::size_t idx = l_idx + r_idx;
            if (idx >= num_blocks)
                continue;

            // create a partially multiplied number and accumulate it
            integer::block_t x[2];
            mul_blocks(l_blocks[l_idx], r_blocks[r_idx], x);
            add(&blocks[idx], num_blocks - idx, x, 2);
        }
    }
}

integer integer::operator*(const integer& rhs) const {
    integer n(this->get_num_blocks(), {});
    mul(*this, rhs, n.get_blocks(), n.get_num_blocks());
    return n;
}

struct div_solution {
    integer q; // quotient
    integer r; // remainder
};

static div_solution div(const integer& lhs, const integer& rhs) {
    const int lhs_msb = lhs.most_significant_active_bit();
    const int rhs_msb = rhs.most_significant_active_bit();
    if (rhs_msb == 0)
        throw std::out_of_range("Divided by zero");

    div_solution sol {
        integer(lhs.get_num_blocks(), {}),
        integer(lhs),
    };
    for (int b = lhs_msb - rhs_msb; b >= 0; b--) {
        // TODO: consider appropriate digit of x
        integer x = rhs * integer::pow2(b);
        if (sol.r >= x) {
            sol.q.set_bit_value(b, true);
            sol.r -= x;
            if (sol.r.is_zero())
                break;
        }
    }
    return sol;
}

integer integer::operator/(const integer& rhs) const {
    return div(*this, rhs).q;
}

integer integer::operator%(const integer& rhs) const {
    return div(*this, rhs).r;
}

bool static is_all_zero(const integer::block_t* blocks, const int num) {
    for (int i = 0; i < num; i++) {
        if (blocks[i] != 0)
            return false;
    }
    return true;
}

struct compare_param {
    bool wider_blocks_is_non_zero;
    bool lhs_is_greater;
    bool rhs_is_greater;
    bool lhs_equals_to_rhs;
};

// lhs should be wider or equals to rhs
static bool compare(const integer& lhs, const integer& rhs, const compare_param& param) {
    const int lhs_num_blocks = lhs.get_num_blocks();
    const int rhs_num_blocks = rhs.get_num_blocks();
    const int num_common_blocks = rhs_num_blocks;
    assert(lhs_num_blocks >= rhs_num_blocks);

    const integer::block_t* lhs_blocks = lhs.ref_blocks();
    const integer::block_t* rhs_blocks = rhs.ref_blocks();

    const int num_wider_blocks = lhs_num_blocks - num_common_blocks;
    if (!is_all_zero(&lhs_blocks[num_common_blocks], num_wider_blocks))
        return param.wider_blocks_is_non_zero;

    for (int i = 0; i < num_common_blocks; i++) {
        const int idx = num_common_blocks - i - 1;
        if (lhs_blocks[idx] > rhs_blocks[idx])
            return param.lhs_is_greater;
        if (lhs_blocks[idx] < rhs_blocks[idx])
            return param.rhs_is_greater;
    }
    return param.lhs_equals_to_rhs;
}

static compare_param cmp_param_eq = {false, false, false, true};

bool static is_equal(const integer& lhs, const integer& rhs) {
    return compare(lhs, rhs, cmp_param_eq);
}

bool integer::operator==(const integer& r) const {
    return (get_num_blocks() > r.get_num_blocks()) ? is_equal(*this, r) : is_equal(r, *this);
}

bool integer::operator!=(const integer& r) const {
    return !((*this) == r);
}

static compare_param cmp_param_gt_eq = {true, true, false, true};

static bool is_gt_eq(const integer& lhs, const integer& rhs) {
    return compare(lhs, rhs, cmp_param_gt_eq);
}

static compare_param cmp_param_lt_eq = {false, false, true, true};

static bool is_lt_eq(const integer& lhs, const integer& rhs) {
    return compare(lhs, rhs, cmp_param_lt_eq);
}

bool integer::operator>=(const integer& r) const {
    return (get_num_blocks() >= r.get_num_blocks()) ? is_gt_eq(*this, r) : is_lt_eq(r, *this);
}

bool integer::operator<=(const integer& r) const {
    return (get_num_blocks() >= r.get_num_blocks()) ? is_lt_eq(*this, r) : is_gt_eq(r, *this);
}

static void bitwise_and(const integer& lhs, const integer& rhs, integer::block_t* blocks) {
    const int num_common_blocks = rhs.get_num_blocks();
    const integer::block_t* lhs_blocks = lhs.ref_blocks();
    const integer::block_t* rhs_blocks = rhs.ref_blocks();
    for (int i = 0; i < num_common_blocks; i++)
        blocks[i] = lhs_blocks[i] & rhs_blocks[i];
}

integer integer::operator&(const integer& r) const {
    const bool this_is_wider = (get_num_blocks() >= r.get_num_blocks());
    const integer& wider = this_is_wider ? *this : r;
    const integer& other = this_is_wider ? r : *this;
    integer n(wider.get_num_blocks(), {0});
    integer::block_t* blocks = n.get_blocks();
    bitwise_and(wider, other, blocks);
    return n;
}

integer& integer::operator<<=(const unsigned int r) {
    const int shift_bits = r % BlockBits;
    block_t* blocks = get_blocks();
    for (int i = get_num_blocks() - 1; i >= 0; i--) {
        const int src_upper_idx = i - (r / BlockBits);
        const int src_lower_idx = src_upper_idx - 1;
        const block_t x1 = src_upper_idx >= 0
                           ? blocks[src_upper_idx] << shift_bits
                           : 0;
        const block_t x0 = (shift_bits != 0) && (src_lower_idx >= 0)
                           ? blocks[src_lower_idx] >> (BlockBits - shift_bits)
                           : 0;
        blocks[i] = x1 | x0;
    }
    return *this;
}

integer& integer::operator>>=(const unsigned int r) {
    const int shift_bits = r % BlockBits;
    block_t* blocks = get_blocks();
    const std::size_t num_blocks = get_num_blocks();
    for (size_t i = 0; i < num_blocks; i++) {
        const int src_lower_idx = i + (r / BlockBits);
        const int src_upper_idx = src_lower_idx + 1;
        const block_t x1 = (shift_bits != 0) && is_valid_index(num_blocks, src_upper_idx)
                           ? blocks[src_upper_idx] << (BlockBits - shift_bits)
                           : 0;
        const block_t x0 = is_valid_index(num_blocks, src_lower_idx)
                           ? blocks[src_lower_idx] >> shift_bits
                           : 0;
        blocks[i] = x1 | x0;
    }
    return *this;
}

integer& integer::operator++() {
    (*this) += constant::One;
    return *this;
}

static int get_most_significant_active_bit(const integer::block_t blk) {
    int width = integer::BlockBits;
    int idx = 0;
    while (width > 0) {
        width >>= 1;
        idx += width;
        if (blk < BitMask[idx])
            idx -= width;
    }
    return idx + 1;
}

int integer::most_significant_active_bit() const {
    const std::size_t num_blocks = get_num_blocks();
    const block_t* blocks = ref_blocks();
    for (int blk_idx = num_blocks - 1; blk_idx >= 0; blk_idx--) {
        const block_t blk = blocks[blk_idx];
        if (blk == 0)
            continue;
        return blk_idx * BlockBits + get_most_significant_active_bit(blk);
    }
    return 0;
}

bool integer::get_bit_value(const int b) const {
    const size_t block_idx = b / BlockBits;
    const size_t mask_idx = b % BlockBits;
    if (block_idx >= get_num_blocks()) {
        std::stringstream ss;
        ss << "Out of range: bit: " << b << ", blocks: " << get_num_blocks();
        THROW_ERROR(ss.str().c_str());
    }
    return this->blocks[block_idx] & BitMask[mask_idx];
}

integer& integer::set_bit_value(const int b, const bool v) {
    const size_t block_idx = b / BlockBits;
    const size_t mask_idx = b % BlockBits;
    if (block_idx >= get_num_blocks()) {
        std::stringstream ss;
        ss << "Out of range: bit: " << b << ", blocks: " << get_num_blocks();
        THROW_ERROR(ss.str().c_str());
    }
    if (v)
        this->blocks[block_idx] |= BitMask[mask_idx];
    else
        this->blocks[block_idx] &= ~BitMask[mask_idx];
    return *this;
}

template<bool MODULO>
integer pow_template(const integer& base, const integer& e, const integer& mod) {
    if (base.get_num_blocks() != 1)
        THROW_ERROR("Not implmented yet: pow()");

    const int most_significant_active_bit = e.most_significant_active_bit();
    integer n(base.get_num_blocks(), {1});
    integer x = base;
    for (int b = 0; b < most_significant_active_bit; b++) {
        if (e.get_bit_value(b)) {
            n *= x;
            if (MODULO)
                n %= mod;
        }
        x *= x;
        if (MODULO)
            x %= mod;
    }
    return n;
}

integer integer::pow(const integer& e) const {
    return pow_template<false>(*this, e, constant::One);
}

integer integer::pow_mod(const integer& e, const integer& mod) const {
    return pow_template<true>(*this, e, mod);
}

integer integer::pow2(const int e) {
    const int num_blocks = e / BlockBits + 1;
    integer n(num_blocks);

    block_t* blocks = n.get_blocks();
    const int idx = e % BlockBits;
    blocks[num_blocks-1] = BitMask[idx];

    const size_t zero_fill_size = (num_blocks - 1) * sizeof(integer::block_t);
    std::memset(blocks, 0, zero_fill_size);

    return n;
}

bool integer::is_odd() const {
    return this->blocks[0] & 1;
}

bool integer::is_even() const {
    return !is_odd();
}

bool integer::is_zero() const {
    return is_all_zero(this->blocks, this->num_blocks);
}

} // namespace grill
