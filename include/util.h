#pragma once
#include "Integer.h"

namespace grill {
namespace util {

/**
 * Return the gratest common divisor.
 *
 * @param a One Integer.
 * @param b Other Integer.
 * @return The greatest common divisor of a and b.
 */
Integer gcd(const Integer& a, const Integer& b);

/**
 * Convert a bool value to "true" or "false".
 *
 * @param b A bool value.
 * @return "true" or "false".
 */
std::string to_string(const bool b);

/**
 * Convert an Integer instance to an unsigned long.
 *
 * The part over the range of an unsigned int is ignored.
 *
 * @param n An Integer.
 * @return A converted unsigned int.
 */
unsigned long to_uint(const Integer& n);

/**
 * Convert a number string to Integer.
 *
 * If a number string begins from 0x, it is handled as a hexadecimal number.
 *
 * @param s A number string.
 */
Integer to_Integer(const std::string& s);

/**
 * Return a random number.
 *
 * @param bit_length A bit length of the created number.
 * @return The random number.
 */
Integer get_random(const std::size_t bit_length);

/**
 * Return a random prime number.
 *
 * @param bit_length A bit length of the created prime number. It must be greater than 2.
 * @return The random prime number.
 */
Integer get_random_prime(const std::size_t bit_length);

} // namespace util
} // namespace grill
