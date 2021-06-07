#include <vector>
#include "primality.h"

namespace grill {

template<typename T>
bool templated_trivial_division(const T& n, const T& zero, const T& two, const T& three) {
    if (n == two)
        return true;
    if (n % two == zero)
        return false;

    std::vector<T> prime_numbers = {};
    auto is_prime = [&](const T& i) {
        for (const auto& prime: prime_numbers) {
            if ((i % prime) == zero)
                return false;
        }
        return true;
    };

    for (T i = three; (i * i) <= n; i += two) {
        if (is_prime(i)) {
            prime_numbers.emplace_back(i);
            if ((n % i) == zero)
                return false;
        }
    }
    return true;
}

bool primality::trivial_division(const integer::block_t n) {
    return templated_trivial_division<integer::block_t>(n, 0, 2, 3);
}

bool primality::trivial_division(const integer& n) {
    return templated_trivial_division<integer>(n, constant::Zero, constant::Two, constant::Three);
}

static bool pass_fermat_little_theorem(const integer& n, const integer& a) {
    return (a.pow_mod(n - constant::One, n) == constant::One);
}

static const wide_int<64> fermat_test_data[] = {
    wide_int<64>(2),
    wide_int<64>(3),
    wide_int<64>(5),
    wide_int<64>(7),
    wide_int<64>(11),
    wide_int<64>(13),
    wide_int<64>(17),
    wide_int<64>(19),
    wide_int<64>(23),
    wide_int<64>(29),
};

bool primality::fermat_test(const integer& n) {
    for (const auto& a: fermat_test_data) {
        if (a >= n)
            continue;
        if (!pass_fermat_little_theorem(n, a))
            return false;
    }
    return true;
}

//
// Miller-Rabbin primality test
//
static const auto& miller_rabin_test_bases = fermat_test_data;

struct miller_rabin_factors {
    // n - 1 = (2^s) * d, where n is an odd primary
    std::size_t s = 0;
    integer d;

    miller_rabin_factors(const integer& n)
    : d(n) {
        while (d.is_even()) {
            s++;
            d >>= 1;
        }
    }
};

enum class number_type {
    ProbablePrime,
    Composite,
};

static integer miller_rabin_formula(const integer& a, const integer& exp, const integer& n) {
    return a.pow_mod(exp, n);
}

static number_type do_miller_rabin_test(
        const integer& a, const integer& n, const integer& minus_one,
        const miller_rabin_factors& factors) {
    integer exp(factors.d);
    const integer v0 = miller_rabin_formula(a, exp, n);
    if (v0 == constant::One || v0 == minus_one)
        return number_type::ProbablePrime;

    for (std::size_t r = 1; r < factors.s; r++) {
        exp <<= 1;
        if (miller_rabin_formula(a, exp, n) == minus_one)
            return number_type::ProbablePrime;
    }
    return number_type::Composite;
}

bool primality::miller_rabin_test(const integer& n) {
    const integer minus_one = n - constant::One; // n-1 is congruent to -1 (mod n)
    const miller_rabin_factors factors(minus_one);
    for (const auto& a: miller_rabin_test_bases) {
        if (a >= n)
            break;
        if (do_miller_rabin_test(a, n, minus_one, factors) == number_type::Composite)
            return false;
    }
    return true;
}

} // namespace grill
