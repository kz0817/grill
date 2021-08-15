#include <cassert>
#include "gear.h"
#include "gear-internal.h"
#include "util.h"
#include <stdexcept>
#include <iostream>

namespace grill {

static constexpr uint64_t One = 1;
static constexpr std::size_t Uint64Bits = (sizeof(uint64_t) * 8);

static uint32_t upper(const uint64_t a) {
    return a >> 32;
}

static uint32_t lower(const uint64_t a) {
    return 0xffff'ffff & a;
}

static void add(uint64_t a[2], const uint64_t b) {
    const uint64_t prev_a0 = a[0];
    a[0] += b;
    const bool carried = (a[0] < prev_a0);
    if (carried)
        a[1]++;
}

void gear::mul(uint64_t out[2], const uint64_t in0, const uint64_t in1) {
    const uint64_t in0l = lower(in0);
    const uint64_t in0u = upper(in0);
    const uint64_t in1l = lower(in1);
    const uint64_t in1u = upper(in1);
    const uint64_t x0 = in0l * in1l;
    const uint64_t x2 = in0u * in1u;
    out[0] = x0;
    out[1] = x2;

    const uint64_t x1a = in0l * in1u;
    grill::add(out, (x1a << 32));
    out[1] += upper(x1a);

    const uint64_t x1b = in0u * in1l;
    grill::add(out, (x1b << 32));
    out[1] += upper(x1b);
}

void gear::twos_complement(uint64_t* buf, const std::size_t n) {
    for (std::size_t i = 0; i < n; i++)
        buf[i] = ~buf[i];
    gear::add(buf, n, &One, 1);
}

struct KaratubaInput {
    const std::size_t num_lower;
    const std::size_t num_upper;
    const uint64_t* lower;
    const uint64_t* upper;

    KaratubaInput(const std::size_t num_lower_half, const std::size_t num_upper_half,
                  const uint64_t* in, std::size_t num_in)
    : num_lower((num_in >= num_lower_half) ? num_lower_half : num_in),
      num_upper((num_in > num_lower_half) ? num_in - num_lower_half : 0),
      lower(in),
      upper((this->num_upper > 0) ? &in[this->num_lower] : nullptr) {
    }

    /**
     * @return true if the result is negative. Otherwise true.
     */
    bool lower_minus_upper(uint64_t* out, const std::size_t n) const {
        assert(n == this->num_lower);
        gear::copy(out, this->lower, n);
        if (upper == nullptr)
            return false;

        const bool borrow = gear::sub(out, n, this->upper, this->num_upper);
        if (borrow)
            gear::twos_complement(out, n);
        return borrow;
    }
};

static void karatsuba_calc_x1(uint64_t* x1, const std::size_t num_x1,
                              const uint64_t* x0, const std::size_t num_x0,
                              const uint64_t* x2, const std::size_t num_x2,
                              const KaratubaInput& a, const KaratubaInput& b) {
    assert(num_x1 >= num_x0);
    gear::fill_zero(&x1[num_x0], num_x1 - num_x0);
    gear::copy(x1, x0, num_x0);
    gear::add(x1, num_x1, x2, num_x2);

    // x1 = x0 + x2 - dx, where dx = da * db
    // da = a.lower - a.upper
    // db = b.lower - b.upper
    uint64_t da[a.num_lower];
    uint64_t db[b.num_lower];
    const bool sign_a = a.lower_minus_upper(da, a.num_lower);
    const bool sign_b = b.lower_minus_upper(db, b.num_lower);

    const std::size_t num_dx = a.num_lower + b.num_lower;
    uint64_t dx[num_dx];
    gear::karatsuba(dx, num_dx, da, a.num_lower, db, b.num_lower);

    if (sign_a == sign_b)
        gear::sub(x1, num_x1, dx, num_dx);
    else
        gear::add(x1, num_x1, dx, num_dx);
}

static void karatsuba_add(uint64_t* out, const std::size_t n_out,
                                const uint64_t* x, const std::size_t num_x,
                                const std::size_t offset) {
    gear::add(&out[offset], n_out - offset, x, num_x);
}

void gear::karatsuba(uint64_t* out, const std::size_t n_out,
                     const uint64_t* in0, const std::size_t num_in0,
                     const uint64_t* in1, const std::size_t num_in1) {
    if (num_in0 == 0 || num_in1 == 0) {
        gear::fill_zero(out, n_out);
        return;
    }

    if (num_in0 == 1 && num_in1 == 1) {
        assert(n_out == 2);
        mul(out, *in0, *in1);
        return;
    }

    // Note: num_lower_half >= num_upper_half
    const std::size_t max_num = (num_in0 >= num_in1) ? num_in0 : num_in1;
    const std::size_t num_upper_half = max_num/2;
    const std::size_t num_lower_half = max_num - num_upper_half;

    const KaratubaInput a(num_lower_half, num_upper_half, in0, num_in0);
    const KaratubaInput b(num_lower_half, num_upper_half, in1, num_in1);

    // out = x0 + x1*R + x2*R^2, where R=2^(64*num_lower_half).
    const std::size_t num_x0 = 2 * num_lower_half;
    const std::size_t num_x1 = num_lower_half + num_upper_half + 1;
    const std::size_t num_x2 = n_out - num_x0;
    uint64_t* x0 = out;
    uint64_t* x2 = &out[num_x0];

    karatsuba(x0, num_x0, a.lower, a.num_lower, b.lower, b.num_lower);
    karatsuba(x2, num_x2, a.upper, a.num_upper, b.upper, b.num_upper);

    uint64_t x1[num_x1];
    karatsuba_calc_x1(x1, num_x1, x0, num_x0, x2, num_x2, a, b);
    karatsuba_add(out, n_out, x1, num_x1, num_lower_half);
}

static constexpr std::uint64_t BitMask[] = {
    0x0000'0000'0000'0001, 0x0000'0000'0000'0002, 0x0000'0000'0000'0004, 0x0000'0000'0000'0008,
    0x0000'0000'0000'0010, 0x0000'0000'0000'0020, 0x0000'0000'0000'0040, 0x0000'0000'0000'0080,
    0x0000'0000'0000'0100, 0x0000'0000'0000'0200, 0x0000'0000'0000'0400, 0x0000'0000'0000'0800,
    0x0000'0000'0000'1000, 0x0000'0000'0000'2000, 0x0000'0000'0000'4000, 0x0000'0000'0000'8000,

    0x0000'0000'0001'0000, 0x0000'0000'0002'0000, 0x0000'0000'0004'0000, 0x0000'0000'0008'0000,
    0x0000'0000'0010'0000, 0x0000'0000'0020'0000, 0x0000'0000'0040'0000, 0x0000'0000'0080'0000,
    0x0000'0000'0100'0000, 0x0000'0000'0200'0000, 0x0000'0000'0400'0000, 0x0000'0000'0800'0000,
    0x0000'0000'1000'0000, 0x0000'0000'2000'0000, 0x0000'0000'4000'0000, 0x0000'0000'8000'0000,

    0x0000'0001'0000'0000, 0x0000'0002'0000'0000, 0x0000'0004'0000'0000, 0x0000'0008'0000'0000,
    0x0000'0010'0000'0000, 0x0000'0020'0000'0000, 0x0000'0040'0000'0000, 0x0000'0080'0000'0000,
    0x0000'0100'0000'0000, 0x0000'0200'0000'0000, 0x0000'0400'0000'0000, 0x0000'0800'0000'0000,
    0x0000'1000'0000'0000, 0x0000'2000'0000'0000, 0x0000'4000'0000'0000, 0x0000'8000'0000'0000,

    0x0001'0000'0000'0000, 0x0002'0000'0000'0000, 0x0004'0000'0000'0000, 0x0008'0000'0000'0000,
    0x0010'0000'0000'0000, 0x0020'0000'0000'0000, 0x0040'0000'0000'0000, 0x0080'0000'0000'0000,
    0x0100'0000'0000'0000, 0x0200'0000'0000'0000, 0x0400'0000'0000'0000, 0x0800'0000'0000'0000,
    0x1000'0000'0000'0000, 0x2000'0000'0000'0000, 0x4000'0000'0000'0000, 0x8000'0000'0000'0000,
};

static std::size_t get_most_significant_active_bit(const std::uint64_t blk) {
    constexpr int WidthVector[] = {32, 16, 8, 4, 2, 1};
    constexpr int NumLoops = sizeof(WidthVector) / sizeof(int);
    int idx = 0;
    for (int i = 0; i < NumLoops; i++) {
        const int trial_idx = idx + WidthVector[i];
        if (blk >= BitMask[trial_idx])
            idx = trial_idx;
    }
    return idx;
}

static constexpr std::size_t NOT_FOUND = -1;

static std::size_t get_first_non_zero_index(const std::uint64_t* n, const std::size_t num_n) {
    for (std::size_t i = 0; i < num_n; i++) {
        const std::size_t idx = num_n - i - 1;
        if (n[idx] != 0)
            return idx;
    }
    return NOT_FOUND;
}

gear::initial_inverse_struct gear::calc_initial_inverse(const std::uint64_t* n,
                                                        const std::size_t num_n) {
    assert(num_n >= 1);

    const std::size_t head_idx = get_first_non_zero_index(n, num_n);
    if (head_idx == NOT_FOUND)
        throw std::invalid_argument("Zero is given for calc_initial_inverse().");

    const uint64_t head = n[head_idx];
    const std::size_t most_significant_bit = get_most_significant_active_bit(head);
    const bool remaining_is_zero = get_first_non_zero_index(n, head_idx) == NOT_FOUND;
    const bool first_bit_is_only_active = ((~BitMask[most_significant_bit] & head) == 0)
                                          && remaining_is_zero;
    const std::size_t shift_adj = first_bit_is_only_active ? 1 : 0;

    gear::initial_inverse_struct init_inv;
    init_inv.value = 0x8000'0000'0000'0000 >> (most_significant_bit - shift_adj);
    init_inv.scale = head_idx + ((head == 1 && first_bit_is_only_active) ? 0 : 1);
    return init_inv;
}

static std::size_t calc_inverse(uint64_t* out, const std::size_t n_out,
                                const uint64_t* n, const std::size_t num_n,
                                const uint64_t* init, const std::size_t num_init,
                                const std::size_t in_scale) {
    // z = out * S = init * (2*S - n * init) = init * (t - u)
    // Note: init: multiplied by S = 2^(64*in_scale)

    // t: 2 * S
    const std::size_t num_t = 1 + in_scale;
    uint64_t t[num_t];
    t[num_t-1] = 2;
    gear::fill_zero(t, num_t-1);

    // u: n * init
    const std::size_t num_u = num_n + num_init;
    uint64_t u[num_u];
    gear::karatsuba(u, num_u, n, num_n, init, num_init);

    // 2*S - n*init
    assert(num_t >= num_u);
    gear::sub(t, num_t, u, num_u);

    // z: out * S
    const std::size_t num_z = num_init + num_t;
    uint64_t z[num_z];
    gear::karatsuba(z, num_z, init, num_init, t, num_t);

    // copy to the output buffer
    const std::size_t out_scale = in_scale;
    assert(num_z - out_scale >= n_out);
    //std::cout << "z: " << gear::to_string(std::vector(z, z + num_z)) << std::endl;
    gear::copy(out, &z[out_scale], n_out);

    return out_scale;
}

static void validate_inverse_out_bits(const std::size_t n_out, const std::size_t significant_bits) {
    const std::size_t max_out_bits = n_out * Uint64Bits;
    if (significant_bits > max_out_bits)
        throw std::invalid_argument("n_out: too short.");
}

struct identical_bits_struct {
    std::size_t bits;
    bool fully;
};

static identical_bits_struct count_identical_bits(const uint64_t* a, const std::size_t num_a,
                                                  const uint64_t* b, const std::size_t num_b) {
    const std::size_t num = std::min(num_a, num_b);
    std::size_t cnt = 0;
    int idx = num - 1;
    bool is_different = false;
    for (; idx >= 0; idx--) {
        is_different = (a[idx] != b[idx]);
        //std::cout << "1: idx: " << idx << ", is_different: " << is_different <<
        //    ", a[idx]: " << std::hex << a[idx] << ", b[idx]: " << b[idx] << std::endl;
        if (is_different)
            break;
        else
            cnt += Uint64Bits;
    }
    //std::cout << "2: cnt: " << std::dec << cnt << std::endl;

    if (is_different) {
        const uint64_t ai = a[idx];
        const uint64_t bi = b[idx];
        for (std::size_t bit = 0; bit < Uint64Bits; bit++) {
            // TODO: use more efficient way.
            const uint64_t a_lsb = ai >> (Uint64Bits - bit - 1);
            const uint64_t b_lsb = bi >> (Uint64Bits - bit - 1);
            if (a_lsb != b_lsb)
                break;
            cnt++;
        }
    }

    identical_bits_struct result;
    result.bits = cnt;
    result.fully = !is_different;
    return result;
}

static std::size_t get_first_one_position(const uint64_t* n, const std::size_t num_n) {
    std::size_t pos = 0;
    int idx = num_n - 1;
    for (; idx > 0; idx--) {
        if (n[idx] == 0)
            pos += Uint64Bits;
    }
    if (idx >= 0) {
        uint64_t mask = 0x8000'0000'0000'0000;
        for (std::size_t bit = 0; bit < Uint64Bits; bit++) {
            // TODO: use more efficient way.
            if ((n[idx] & mask) == 0)
                pos++;
            else
                break;
            mask >>= 1;
        }
    }
    return pos;
}

std::size_t gear::inverse(uint64_t* out, const std::size_t n_out,
                          const uint64_t* in, const std::size_t num_in,
                          const std::size_t significant_bits) {
    validate_inverse_out_bits(n_out, significant_bits);
    const initial_inverse_struct initial_val = calc_initial_inverse(in, num_in);

    const std::size_t num_init = 1; // tentative
    uint64_t init[num_init] = {initial_val.value};
    std::size_t in_scale = initial_val.scale;
    std::size_t out_scale = 0;
    std::size_t prev_first_one_position = get_first_one_position(init, num_init);
    while (true) {
        out_scale = calc_inverse(out, n_out, in, num_in, init, num_init, in_scale);
        std::cout << "out: " << to_string(std::vector(out, out + n_out)) <<
            ", out_scale: " << out_scale << ", in: " <<
            to_string(std::vector(init, init + num_init)) <<
            ", in_scale: " << in_scale <<
            ", significant_bits: " << significant_bits << std::endl;

        const identical_bits_struct identical = count_identical_bits(init, num_init, out, n_out);
        if (identical.fully)
            break;

        const std::size_t first_one_position = get_first_one_position(out, n_out);
        //std::cout << "identical.bits: " << identical.bits << ", first_one_pos: " <<
        //    first_one_position << ", prev_pos: " << prev_first_one_position << std::endl;
        // identical_bits - first_one_position >= significant_bits
        const bool enough_significant_bits =
            identical.bits >= significant_bits + first_one_position;
        if ((first_one_position == prev_first_one_position) && enough_significant_bits)
            break;

        prev_first_one_position = first_one_position;
        gear::copy(init, out, num_init); // TODO: Is this correnct
        in_scale = out_scale;
    }
    return out_scale;
}

/*
void gear::div_newton_raphson(uint64_t* q, const std::size_t num_q,
                              const uint64_t* a, const std::size_t num_a,
                              const uint64_t* b, const std::size_t num_b) {
    const std::size_t scale = 1; // tentative
    const std::size_t num_inv = 2; // tentative
    uint64_t inv[num_inv];
    calc_inverse(inv, num_inv, b, num_b, &One, 1, scale);
    karatsuba(q, num_q, a, num_a, inv, num_inv);
}*/

} // namespace grill
