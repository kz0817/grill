#include <cassert>
#include "gear.h"

namespace grill {

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

} // namespace grill
