#include <ostream>
#include <vector>
#include "primality.h"
#include "constant.h"

namespace grill {

static bool is_zero(const Integer::block_t n) {
    return n == 0;
}

static bool is_zero(const Integer& n) {
    return n.is_zero();
}

template<typename T>
bool templated_trivial_division(const T& n, const T& two, const T& three) {
    if (n == two)
        return true;
    if (is_zero(n % two))
        return false;

    std::vector<T> prime_numbers = {};
    auto is_prime = [&](const T& i) {
        for (const auto& prime: prime_numbers) {
            if (is_zero(i % prime))
                return false;
        }
        return true;
    };

    for (T i = three; (i * i) <= n; i += two) {
        if (is_prime(i)) {
            prime_numbers.emplace_back(i);
            if (is_zero(n % i))
                return false;
        }
    }
    return true;
}

bool primality::trivial_division(const Integer::block_t n) {
    return templated_trivial_division<Integer::block_t>(n, 2, 3);
}

bool primality::trivial_division(const Integer& n) {
    return templated_trivial_division<Integer>(n, constant::Two, constant::Three);
}

static bool pass_fermat_little_theorem(const Integer& n, const Integer& a) {
    return (a.pow_mod(n - constant::One, n) == constant::One);
}

static const Integer fermat_test_data[] = {
    Integer({2}),
    Integer({3}),
    Integer({5}),
    Integer({7}),
    Integer({11}),
    Integer({13}),
    Integer({17}),
    Integer({19}),
    Integer({23}),
    Integer({29}),
};

bool primality::fermat_test(const Integer& n) {
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

struct MillerRabinFactors {
    // n - 1 = (2^s) * d, where n is an odd primary
    std::size_t s = 0;
    Integer d;

    MillerRabinFactors(const Integer& n)
    : d(n) {
        while (d.is_even()) {
            s++;
            d >>= 1;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const MillerRabinFactors& f) {
        os << "s: " << f.s << ", d: " << f.d;
        return os;
    }
};

enum class NumberType {
    ProbablePrime,
    Composite,
};

static Integer miller_rabin_formula(const Integer& a, const Integer& exp, const Integer& n) {
    return a.pow_mod(exp, n);
}

static NumberType do_miller_rabin_test(
        const Integer& a, const Integer& n, const Integer& minus_one,
        const MillerRabinFactors& factors) {
    Integer exp(factors.d);
    const Integer v0 = miller_rabin_formula(a, exp, n);
    if (v0 == constant::One || v0 == minus_one)
        return NumberType::ProbablePrime;

    for (std::size_t r = 1; r < factors.s; r++) {
        exp <<= 1;
        if (miller_rabin_formula(a, exp, n) == minus_one)
            return NumberType::ProbablePrime;
    }
    return NumberType::Composite;
}

bool primality::miller_rabin_test(const Integer& n) {
    const Integer minus_one = n - constant::One; // n-1 is congruent to -1 (mod n)
    const MillerRabinFactors factors(minus_one);
    for (const auto& a: miller_rabin_test_bases) {
        if (a >= n)
            break;
        if (do_miller_rabin_test(a, n, minus_one, factors) == NumberType::Composite)
            return false;
    }
    return true;
}

} // namespace grill
