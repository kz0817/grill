#include <cassert>
#include "gear.h"
#include "util.h"

namespace grill {

static constexpr uint64_t One = 1;

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

} // namespace grill
