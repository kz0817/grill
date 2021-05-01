#include <stdexcept>
#include "integer.h"

integer::operator std::string() const {
    if (num_chunks() != 1)
        throw std::logic_error("Not implmented yet");

    return std::to_string(ref_chunks()[0]);
}

void integer::add(const integer& n) {
    if (num_chunks() != 1)
        throw std::logic_error("Not implmented yet");

    get_chunks()[0] += n.ref_chunks()[0];
}

void integer::sub(const integer& n) {
    if (num_chunks() != 1)
        throw std::logic_error("Not implmented yet");

    get_chunks()[0] -= n.ref_chunks()[0];
}

void integer::mul(const integer& n) {
    if (num_chunks() != 1)
        throw std::logic_error("Not implmented yet");

    get_chunks()[0] *= n.ref_chunks()[0];
}

void integer::div(const integer& n) {
    if (num_chunks() != 1)
        throw std::logic_error("Not implmented yet");

    get_chunks()[0] /= n.ref_chunks()[0];
}

void integer::mod(const integer& n) {
    if (num_chunks() != 1)
        throw std::logic_error("Not implmented yet");

    get_chunks()[0] %= n.ref_chunks()[0];
}
