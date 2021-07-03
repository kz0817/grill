#include <algorithm>
#include <stdexcept>
#include <random>
#include "constant.h"
#include "util.h"
#include "primality.h"

namespace grill {

struct IntegerGenerator : public Integer {
    IntegerGenerator(const std::size_t n_blk, const block_t* const src)
    : Integer(n_blk) {
        gear::copy(this->get_blocks(), src, n_blk);
    }
};

// a should be greater than or equal to  b
static Integer calc_gcd(const Integer& a, const Integer& b) {
    const Integer r = a % b;
    return (r == constant::Zero) ? b : calc_gcd(b, r);
}

Integer util::gcd(const Integer& a, const Integer& b) {
    return (a >= b) ? calc_gcd(a, b) : calc_gcd(b, a);
}

std::string util::to_string(const bool b) {
    return b ? "true" : "false";
}

unsigned long util::to_uint(const Integer& n) {
    return n.ref_blocks()[0];
}

static const std::string HexPrefix = "0x";

static bool has_hex_prefix(const std::string& s) {
    return s.compare(0, HexPrefix.size(), HexPrefix) == 0;
}

static bool is_valid_hex_number(const std::string& s) {
    const auto found_pos = std::find_if(s.begin(), s.end(), [](const char c) {
        return !(c >= '0' && c <= '9') &&
               !(c >= 'a' && c <= 'f') &&
               !(c >= 'A' && c <= 'F');
    });
    return found_pos == s.end();
}

static bool is_valid_dec_number(const std::string& s) {
    const auto found_pos = std::find_if(s.begin(), s.end(), [](const char c) {
        return !(c >= '0' && c <= '9');
    });
    return found_pos == s.end();
}

static Integer hex_str_to_Integer(const std::string& hex_str) {
    constexpr std::size_t str_len_per_block = sizeof(Integer::block_t) * 2;
    const std::size_t msb_digits = hex_str.size() % str_len_per_block;
    const std::size_t num_blocks = hex_str.size() / str_len_per_block + (msb_digits ? 1 : 0);
    Integer::block_t blocks[num_blocks];

    std::size_t pos = 0;
    const auto fill_block = [&](const int str_chunk_idx, const std::size_t len) {
        const std::string blk = hex_str.substr(pos, len);
        const std::size_t blk_idx = num_blocks - str_chunk_idx - 1;
        blocks[blk_idx] = std::stoul(blk, nullptr, 16);
        pos += len;
    };

    fill_block(0, msb_digits == 0 ? str_len_per_block : msb_digits);
    for (std::size_t i = 1; i < num_blocks; i++)
        fill_block(i, str_len_per_block);

    return IntegerGenerator(num_blocks, blocks);
}

static Integer hex_str_with_prefix_to_Integer(const std::string& s) {
    const std::string hex_str = s.substr(HexPrefix.size());
    if (!is_valid_hex_number(hex_str))
        throw std::invalid_argument(hex_str);
    return hex_str_to_Integer(hex_str);
}

static Integer dec_str_to_Integer(const std::string& s) {
    if (!is_valid_dec_number(s))
        throw std::invalid_argument(s);
    // TODO: support more digit
    return Integer({std::stoul(s)});
}

Integer util::to_Integer(const std::string& s) {
    if (has_hex_prefix(s))
        return hex_str_with_prefix_to_Integer(s);

    return dec_str_to_Integer(s);
}

Integer util::get_random(const std::size_t bit_length) {
    std::random_device seed_gen;
    std::mt19937_64 engine(seed_gen());
    constexpr int RANDOM_ENGINE_SIZE = 64;
    static_assert(RANDOM_ENGINE_SIZE == Integer::BlockBits);

    const std::size_t remaining_bits = bit_length % Integer::BlockBits;
    const std::size_t quotient = bit_length / Integer::BlockBits;
    const std::size_t num_blocks = remaining_bits == 0 ? quotient : quotient + 1;
    Integer::block_t buf[num_blocks];

    for (std::size_t i = 0; i < num_blocks; i++)
        buf[i] = engine();

    if (remaining_bits > 0) {
        const std::size_t shift_bits = Integer::BlockBits - remaining_bits;
        buf[num_blocks-1] >>= shift_bits;
    }
    return IntegerGenerator(num_blocks, buf);
}

Integer util::get_random_prime(const std::size_t bit_length) {
    if (bit_length <= 2)
        throw std::invalid_argument("bit_length must be greater than 2");

    Integer n = constant::Zero;
    while (true) {
        n = get_random(bit_length);
        if (n <= constant::Two)
            continue;
        if (primality::miller_rabin_test(n))
            break;
    }
    return n;
}

} // namespace grill
