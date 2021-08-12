#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>

namespace grill {
namespace gear {

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

template<typename T>
std::string to_string(const std::vector<T>& vect) {
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

template<typename T>
struct AddOp {
    static bool calc(T& lhs, const T& rhs, const T prev_lhs) {
        lhs += rhs;
        return lhs < prev_lhs;
    }

    static bool handle_carry(T& lhs) {
        lhs++;
        return lhs == 0;
    }
};

template<typename T>
struct SubOp {
    static bool calc(T& lhs, const T& rhs, const T prev_lhs) {
        lhs -= rhs;
        return lhs > prev_lhs;
    }

    static bool handle_carry(T& lhs) {
        lhs--;
        return lhs == static_cast<T>(-1);
    }
};

template <typename T, typename OP>
bool do_add_sub(T* dest, const std::size_t num_dest, const T* in, const std::size_t num_in) {
    bool carry_flag = false;
    for (std::size_t i = 0; i < num_dest; i++) {
        const bool in_is_valid = (i < num_in);
        if (!in_is_valid && !carry_flag)
            break;

        if (carry_flag)
            carry_flag = OP::handle_carry(dest[i]);

        const T in_val = in_is_valid ? in[i] : 0;
        carry_flag |= OP::calc(dest[i], in_val, dest[i]);
    }
    return carry_flag;
}

/**
 * Add the number.
 *
 * @tparam T A type of the block.
 * @param dest The blocks to be added. Least significant block first.
 * @param num_dest The number of blocks of `dest`.
 * @param in The blocks to add.
 * @param num_in The number of blocks of `in`.
 * @return A carry flag.
 */
template <typename T>
bool add(T* dest, const std::size_t num_dest, const T* in, const std::size_t num_in) {
    return do_add_sub<T, AddOp<T>>(dest, num_dest, in, num_in);
}

/**
 * Subtract the number.
 *
 * @tparam T A type of the block.
 * @param dest The blocks to be subtracted. Least significant block first.
 * @param num_dest The number of blocks of `dest`.
 * @param in The blocks to add.
 * @param num_in The number of blocks of `in`.
 * @return A borrow flag.
 */
template <typename T>
bool sub(T* dest, const std::size_t num_dest, const T* in, const std::size_t num_in) {
    return do_add_sub<T, SubOp<T>>(dest, num_dest, in, num_in);
}

/**
 * Multiply two numbers.
 *
 * @param out the output buffer.
 * @param in0 A number.
 * @param in1 An another number.
 */
void mul(uint64_t out[2], const uint64_t in0, const uint64_t in1);

} // namespace gear
} // namespace grill

