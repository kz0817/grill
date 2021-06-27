#include "rsa.h"
#include "constant.h"
#include "util.h"

namespace grill {
namespace rsa {

Keys generate_keys(const std::size_t bit_length) {
    Integer prime1 = util::get_random_prime(bit_length/2);
    Integer prime2 = util::get_random_prime(bit_length/2);
    Integer modulus = prime1 * prime2;
    Integer phi = (prime1 - constant::One) * (prime2 - constant::One);
    Integer pub_key = WideInt<64>(0x10001);
    Integer priv_key = pub_key.inverse(phi);

    return Keys {
        std::move(prime1), std::move(prime2), std::move(phi),
        std::move(modulus), std::move(pub_key), std::move(priv_key),
    };
}

Integer compute(const Integer& exponent, const Integer& modulus, const Integer& n) {
    return n.pow_mod(exponent, modulus);
}

} // namespace rsa
} // namespace grill
