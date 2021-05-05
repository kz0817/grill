#include "integer.h"
#include "constant.h"

const wide_int<64> constant::zero = wide_int<64>(static_cast<integer::block_t>(0));
const wide_int<64> constant::one = wide_int<64>(1);
