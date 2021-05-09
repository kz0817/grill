#include <vector>
#include "primality.h"

namespace grill {


bool primality::trivial_division(const integer::block_t n) {

    std::vector<integer::block_t> prime_numbers;
    auto is_prime = [&](integer::block_t i) {
        for (const auto& prime: prime_numbers) {
            if ((i % prime) == 0)
                return false;
        }
        return true;
    };

    for (integer::block_t i = 2; (i * i) <= n; i++) {
        if (is_prime(i))
            prime_numbers.emplace_back(i);
    }
    return is_prime(n);
}

static bool pass_fermat_little_theorem(const integer& n, const integer& a) {
    return ((a.pow(n - constant::One) % n) == constant::One);
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
        if (util::gcd(a, n) != constant::One)
             return false;
        if (!pass_fermat_little_theorem(n, a))
            return false;
    }
    return true;
}

} // namespace grill
