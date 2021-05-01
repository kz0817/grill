#include <stdexcept>
#include "integer.h"

integer::operator std::string() const {
    if (num_buf() != 1)
        throw std::logic_error("Not implmented yet");

    return std::to_string(ref_buf()[0]);
}
