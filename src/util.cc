#include "constant.h"
#include "util.h"

// a should be greater than or equal to  b
static integer calc(const integer& a, const integer& b) {
    integer q = a / b;
    integer r = a % b;
    if (r == constant::zero)
        return b;
    if (r == constant::one)
        return constant::one;
    return calc(b, r);
}

integer util::gcd(const integer& a, const integer& b) {
    return (a >= b) ? calc(a, b) : calc(b, a);
}
