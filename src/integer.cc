#include <stdexcept>
#include <cstring>
#include "integer.h"

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
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    return std::to_string(ref_blocks()[0]);
}

std::ostream& operator<<(std::ostream& os, const integer& data) {
    os << static_cast<std::string>(data);
    return os;
}

integer& integer::add(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] += n.ref_blocks()[0];
    return *this;
}

integer& integer::sub(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] -= n.ref_blocks()[0];
    return *this;
}

integer& integer::mul(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] *= n.ref_blocks()[0];
    return *this;
}

integer& integer::div(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] /= n.ref_blocks()[0];
    return *this;
}

integer& integer::mod(const integer& n) {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    get_blocks()[0] %= n.ref_blocks()[0];
    return *this;
}

integer& integer::operator+=(const integer& n) {
    add(n);
    return *this;
}

integer& integer::operator-=(const integer& n) {
    sub(n);
    return *this;
}

integer& integer::operator*=(const integer& n) {
    mul(n);
    return *this;
}

integer& integer::operator/=(const integer& n) {
    div(n);
    return *this;
}

integer& integer::operator%=(const integer& n) {
    mod(n);
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

integer integer::pow(const integer& e) const {
    if (get_num_blocks() != 1)
        throw std::logic_error("Not implmented yet");

    block_t cnt = e.ref_blocks()[0];
    integer n = create(1);
    for (block_t i = 0; i < cnt; i++)
        n *= (*this);
    return n;
}
