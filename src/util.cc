#include <algorithm>
#include <stdexcept>
#include "constant.h"
#include "util.h"

namespace grill {

struct integer_generator : public integer {
    integer_generator(const std::size_t n_blk, const block_t* const src)
    : integer(n_blk, src) {
    }
};

// a should be greater than or equal to  b
static integer calc_gcd(const integer& a, const integer& b) {
    const integer r = a % b;
    return (r == constant::Zero) ? b : calc_gcd(b, r);
}

integer util::gcd(const integer& a, const integer& b) {
    return (a >= b) ? calc_gcd(a, b) : calc_gcd(b, a);
}

std::string util::to_string(const bool b) {
    return b ? "true" : "false";
}

unsigned long util::to_uint(const integer& n) {
    return n.ref_blocks()[0];
}

static const std::string HexPrefix = "0x";

static bool has_hex_prefix(const std::string& s) {
    return s.compare(0, HexPrefix.size(), HexPrefix) == 0;
}

static bool is_valid_hex_number(const std::string& s) {
    const auto found_pos = std::find_if(s.begin(), s.end(), [](const char c) {
        return !(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f');
    });
    return found_pos == s.end();
}

static bool is_valid_dec_number(const std::string& s) {
    const auto found_pos = std::find_if(s.begin(), s.end(), [](const char c) {
        return !(c >= '0' && c <= '9');
    });
    return found_pos == s.end();
}

static integer hex_str_to_integer(const std::string& hex_str) {
    constexpr std::size_t str_len_per_block = sizeof(integer::block_t) * 2;
    const std::size_t msb_digits = hex_str.size() % str_len_per_block;
    const std::size_t num_blocks = hex_str.size() / str_len_per_block + (msb_digits ? 1 : 0);
    integer::block_t blocks[num_blocks];

    std::size_t pos = 0;
    const auto fill_block = [&](const int str_chunk_idx, const std::size_t len) {
        const std::string blk = hex_str.substr(pos, len);

        // The order of blocks is the least significant block first.
        const int block_idx = num_blocks - str_chunk_idx - 1;
        blocks[block_idx] = std::stoul(blk);
    };

    fill_block(0, msb_digits == 0 ? str_len_per_block : msb_digits);
    for (std::size_t i = 1; i < num_blocks; i++)
        fill_block(i, str_len_per_block);

    return integer_generator(num_blocks, blocks);
}

static integer hex_str_with_prefix_to_integer(const std::string& s) {
    const std::string hex_str = s.substr(HexPrefix.size());
    if (!is_valid_hex_number(s))
        throw std::invalid_argument(hex_str);
    return hex_str_to_integer(hex_str);
}

static integer dec_str_to_integer(const std::string& s) {
    if (!is_valid_dec_number(s))
        throw std::invalid_argument(s);
    // TODO: support more digit
    return wide_int<64>(std::stoul(s));
}

integer util::to_integer(const std::string& s) {
    if (has_hex_prefix(s))
        return hex_str_with_prefix_to_integer(s);

    return dec_str_to_integer(s);
}

} // namespace grill
