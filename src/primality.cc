#include <vector>
#include "primality.h"

namespace grill {

template<typename T>
bool templated_trivial_division(const T& n, const T& zero, const T& two, const T& three) {
    if (n == two)
        return true;

    std::vector<T> prime_numbers = {two};
    auto is_prime = [&](const T& i) {
        for (const auto& prime: prime_numbers) {
            if ((i % prime) == zero)
                return false;
        }
        return true;
    };

    for (T i = three; (i * i) <= n; i += two) {
        if (is_prime(i))
            prime_numbers.emplace_back(i);
    }
    return is_prime(n);
}

bool primality::trivial_division(const integer::block_t n) {
    return templated_trivial_division<integer::block_t>(n, 0, 2, 3);
}

bool primality::trivial_division(const integer& n) {
    return templated_trivial_division<integer>(n, constant::Zero, constant::Two, constant::Three);
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
