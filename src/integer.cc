#include <stdexcept>
#include <cstring>
#include "integer.h"

integer::integer(const integer& n)
: num_blocks(n.get_num_blocks()),
  blocks(new block_t[this->num_blocks]), //  TODO: use a custom allocator
  blocks_owner(true) {
    std::memcpy(this->blocks, n.blocks, sizeof(block_t) * this->num_blocks);
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

std::size_t integer::get_num_blocks() const {
    return this->num_blocks;
}

integer::block_t* integer::get_blocks() const {
    return this->blocks;
}

const integer::block_t* integer::ref_blocks() const {
    return get_blocks();
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
