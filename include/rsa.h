#pragma once
#include "Integer.h"

namespace grill {
namespace rsa {

struct Keys {
    Integer prime1;
    Integer prime2;
    Integer phi;
    Integer modulus;
    Integer public_exponent;
    Integer private_exponent;
};

/**
 * Generate keys.
 *
 * @param bit_length A bit length of the created keys.
 * @return The generated kyes.
 */
 Keys generate_keys(const std::size_t bit_length);

/**
 * Encrypt or decrypt data.
 *
 * @param exponent An exponent.
 * @param modulus A modulus.
 * @param n An Integer to be encrypted or decrypted.
 * @return The encrypted or decrypted data.
 */
Integer compute(const Integer& exponent, const Integer& modulus, const Integer& n);

} // namespace rsa
} // namespace grill
