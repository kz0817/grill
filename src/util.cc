#include "constant.h"
#include "util.h"

// a should be greater than or equal to  b
static integer calc_gcd(const integer& a, const integer& b) {
    const integer r = a % b;
    return (r == constant::zero) ? b : calc_gcd(b, r);
}

integer util::gcd(const integer& a, const integer& b) {
    return (a >= b) ? calc_gcd(a, b) : calc_gcd(b, a);
}
