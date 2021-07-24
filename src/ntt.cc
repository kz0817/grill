#include <cassert>
#include <cstdint>
#include <sstream>
#include <iostream>

namespace grill {
namespace ntt {

// static const uint64_t MODULUS = 0xffff'ffff'0000'0001; // 2^64 - 2^32 + 1
// MODULUS-1 = 2^32 * 3 * 5 * 17 * 257 * 65537
// primitive root: 7 ^ ((MODULUS-1)/N), where N is the number of division

static const uint64_t MODULUS = 0x20'0000 * 11 + 1; // 2^21 * 11 + 1
static const uint64_t PRIMITIVE_ROOT = 38;
static const int ORDER = 21;

static const int UNIT_BITS = 16;

static const uint64_t ROOT_TABLE[] = {
// r^e (mod m) : e
          38, // 2^0
        1444, // 2^1
     2085136, // 2^2
    16269513, // 2^3
    17172558, // 2^4
     4843647, // 2^5
     6613590, // 2^6
     7628393, // 2^7
    14244166, // 2^8
    19368561, // 2^9
     9691831, // 2^10
    11763009, // 2^11
    11732070, // 2^12
    14437792, // 2^13
    16761576, // 2^14
    14326920, // 2^15
    13517616, // 2^16
    21580741, // 2^17
    18020141, // 2^18
    21304244, // 2^19
    23068672, // 2^20
           1, // 2^21
};

static const uint64_t INV_ROOT_TABLE[] = {
// r^e (mod m) : e
    21247462, // -2^0
    18771254, // -2^1
    22410700, // -2^2
    21751211, // -2^3
    19164924, // -2^4
    21665182, // -2^5
    22205630, // -2^6
     1906025, // -2^7
     7470566, // -2^8
     7831646, // -2^9
    14258338, // -2^10
     1322346, // -2^11
    16598989, // -2^12
    18815717, // -2^13
    18818115, // -2^14
    20098475, // -2^15
    15818506, // -2^16
    20375918, // -2^17
     7261338, // -2^18
     1764429, // -2^19
    23068672, // -2^20
           1, // -2^21
};

static void assert_even(const std::size_t n) {
    if (n % 2 != 0) {
        std::ostringstream oss;
        oss << "Not a power of the two: " << n;
        throw std::invalid_argument(oss.str());
    }
}

static std::size_t log2(const std::size_t n) {
    std::size_t a = 0;
    std::size_t m = n;
    while (m > 1) {
        assert_even(m);
        m >>= 1;
        a++;
    }
    return a;
}

static int get_root_table_index(const std::size_t size_log2) {
    return ORDER - size_log2;
}

static uint64_t minus(const uint64_t a, const uint64_t b, const uint64_t mod) {
    // We assume: 0 <= a < mod
    const uint64_t b_mod = b % mod;
    const uint64_t x = (a >= b_mod) ? (a - b_mod) : ((a + mod) - b_mod);
    return x;
}

static uint64_t pow(const uint64_t a, const std::size_t exp, const uint64_t mod) {
    uint64_t n = 1;
    // TODO: lookup the pre-calculated array.
    for (std::size_t i = 0; i < exp; i++) {
        n *= a;
        n %= mod;
    }
    return n;
}

static void butterfly(const std::uint64_t* in_even, const std::uint64_t* in_odd,
                      std::uint64_t *out, const std::size_t size_half,
                      const uint64_t* root_table, const std::size_t size_log2) {
    for (std::size_t k = 0; k < size_half; k++) {
        const uint64_t w = root_table[get_root_table_index(size_log2)];
        const uint64_t w_k = pow(w, k, MODULUS);
        out[k] = (in_even[k] + w_k * in_odd[k]) % MODULUS;
        out[k+size_half] = minus(in_even[k], w_k * in_odd[k], MODULUS);
    }
}

static void fft(const std::uint64_t* in, std::uint64_t *out, const std::size_t size,
                const uint64_t* root_table, const std::size_t size_log2) {

    if (size == 2) {
        butterfly(&in[0], &in[1], out, 1, root_table, 1);
        return;
    }

    // Prepare the source data for the half-size FFT
    const std::size_t size_half = size / 2;
    uint64_t in_even[size_half];
    uint64_t in_odd[size_half];
    for (std::size_t i = 0; i < size_half; i++) {
        in_even[i] = in[2*i];
        in_odd[i] = in[2*i+1];
    }

    // Calculate the half size of FFT
    uint64_t out_even[size_half];
    uint64_t out_odd[size_half];
    fft(in_even, out_even, size_half, root_table, size_log2-1);
    fft(in_odd,  out_odd,  size_half, root_table, size_log2-1);

    // Calculate the FFT with the above half-size calculation result.
    butterfly(out_even, out_odd, out, size_half, root_table, size_log2);
}

void transform(const std::uint64_t* in, std::uint64_t *out, const std::size_t size) {
    fft(in, out, size, ROOT_TABLE, log2(size));
}

void inverse_transform(const std::uint64_t* in, std::uint64_t *out, const std::size_t size) {
    fft(in, out, size, INV_ROOT_TABLE, log2(size));
    for (std::size_t i = 0; i < size; i++)
        out[i] /= size;
}

static std::size_t calc_num_units(const std::size_t num, const std::size_t unit_bits) {
    const std::size_t data_bits = num * sizeof(uint64_t) * 8;
    const std::size_t n_units = data_bits / unit_bits;
    return (data_bits % unit_bits == 0) ? n_units : (n_units + 1);
}

void split(const uint64_t* in, const std::size_t num_in,
           uint64_t* out, const std::size_t num_out, const std::size_t out_unit_bits) {
    constexpr std::size_t IN_UNIT_BITS = sizeof(decltype(*in)) * 8;
    const uint64_t mask = (1 << out_unit_bits) - 1;
    std::size_t lead_bit = 0;

    if (calc_num_units(num_in, out_unit_bits) < num_out) {
        std::ostringstream oss;
        oss << "The array for the output is too small.";
        throw std::length_error(oss.str());
    }

    for (std::size_t i = 0; i < num_in; i++) {
        uint64_t n = in[i];
        if (lead_bit != 0) {
            // This branch is executed when there are remaining bits in the previous iteration.
            const uint64_t _mask = (1 << lead_bit) - 1;
            *out |= ((n & _mask) << (out_unit_bits - lead_bit));
            out++;
            n >>= lead_bit;
        }

        while (true) {
            *out = (mask & n);
            lead_bit += out_unit_bits;
            n >>= out_unit_bits;
            if (lead_bit >= IN_UNIT_BITS)
                break;
            out++;
        }
        lead_bit %= IN_UNIT_BITS;
        if (lead_bit == 0)
            out++;
    }
}

} // namespace ntt
} // namespace grill
