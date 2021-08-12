#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Integer.h"

std::vector<grill::Integer::block_t> create_block_vector(const grill::Integer& n);

template<typename T>
std::string std::to_string(const std::vector<T>& vect) {
    std::ostringstream oss;
    oss << std::hex << "{";
    for (std::size_t i = 0; i < vect.size(); i++) {
        oss << vect[i];
        if (i < vect.size()-1)
            oss << ", ";
    }
    oss << "}";
    return oss.str();
}
