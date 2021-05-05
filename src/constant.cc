#include "integer.h"
#include "constant.h"

namespace grill {

const wide_int<64> constant::zero = wide_int<64>(static_cast<integer::block_t>(0));
const wide_int<64> constant::one = wide_int<64>(1);

} // namespace grill
