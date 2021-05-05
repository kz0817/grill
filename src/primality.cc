#include "primality.h"

namespace grill {

static bool pass_fermat_little_theorem(const integer& n, const integer& a) {
    return ((a.pow(n - constant::one) % n) == constant::one);
}

static const wide_int<64> fermat_test_data[] = {
    wide_int<64>(2),
    wide_int<64>(3),
    wide_int<64>(5),
};

bool primality::fermat_test(const integer& n) {
    for (const auto& a: fermat_test_data) {
        if (a == n)
            continue;
        if (util::gcd(a, n) != constant::one)
             return false;
        if (!pass_fermat_little_theorem(n, a))
            return false;
    }
    return true;
}

} // namespace grill
