#include <stdexcept>
#include <cstdio>
#include <cstring>
#include <sstream>
#include "integer.h"

namespace grill {

static std::string create_error_msg(const char* const filename, const int lineno,
                                    const char* const msg) {
    std::stringstream ss;
    ss << "[" << filename << ":" << lineno << "] " << msg;
    return ss.str();
}

#define THROW_ERROR(MSG) throw std::runtime_error(create_error_msg(__FILE__, __LINE__, MSG))

integer::integer(const integer& n, const bool skip_blocks_copy)
: num_blocks(n.get_num_blocks()),
  blocks(new block_t[this->num_blocks]), //  TODO: use a custom allocator
  blocks_owner(true) {
    if (!skip_blocks_copy)
        std::memcpy(this->blocks, n.blocks, sizeof(block_t) * this->num_blocks);
}

integer::integer(integer&& n)
: num_blocks(n.num_blocks),
  blocks(n.blocks),
  blocks_owner(n.blocks_owner) {
    if (n.blocks_owner)
        n.blocks_owner = false;
}

integer::integer(const std::size_t num, block_t* const buf)
: num_blocks(num),
  blocks(buf),
  blocks_owner(false) {
}

integer::~integer() {
    if (this->blocks_owner)
        delete [] this->blocks; // TODO: use a custom allocator
}

integer integer::create(const integer::block_t init_value) const {
    const bool skip_block_copy = true;
    integer n(*this, skip_block_copy);
    n = init_value;
    return n;
}

std::size_t integer::get_num_blocks() const {
    return this->num_blocks;
}

integer::block_t* integer::get_blocks() const {
    return this->blocks;
}

const integer::block_t* integer::ref_blocks() const {
    return get_blocks();
}

bool integer::is_blocks_owner() const {
    return this->blocks_owner;
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

integer& integer::operator+=(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] += n.ref_blocks()[0];
    return *this;
}

integer& integer::operator-=(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] -= n.ref_blocks()[0];
    return *this;
}

integer& integer::operator*=(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] *= n.ref_blocks()[0];
    return *this;
}

integer& integer::operator/=(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] /= n.ref_blocks()[0];
    return *this;
}

integer& integer::operator%=(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] %= n.ref_blocks()[0];
    return *this;
}

integer& integer::operator=(const block_t n) {
    this->blocks[0] = n;

    const std::size_t num_zero_blocks = this->num_blocks - 1;
    if (num_zero_blocks >= 1)
        std::memset(&this->blocks[1], 0, sizeof(block_t) * num_zero_blocks);
    return *this;
}

integer integer::operator+(const integer& r) const {
    integer n(*this);
    n += r;
    return n;
}

integer integer::operator-(const integer& r) const {
    integer n(*this);
    n -= r;
    return n;
}

integer integer::operator*(const integer& r) const {
    integer n(*this);
    n *= r;
    return n;
}

integer integer::operator/(const integer& r) const {
    integer n(*this);
    n /= r;
    return n;
}

integer integer::operator%(const integer& r) const {
    integer n(*this);
    n %= r;
    return n;
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
    integer n = wider.create(0);
    integer::block_t* blocks = n.get_blocks();
    bitwise_and(wider, other, blocks);
    return n;
}

integer integer::pow(const integer& e) const {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    block_t cnt = e.ref_blocks()[0];
    integer n = create(1);
    for (block_t i = 0; i < cnt; i++)
        n *= (*this);
    return n;
}

} // namespace grill
