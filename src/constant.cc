#include "integer.h"
#include "constant.h"

namespace grill {

const wide_int<64> constant::Zero = wide_int<64>(static_cast<integer::block_t>(0));
const wide_int<64> constant::One = wide_int<64>(1);
const wide_int<64> constant::Two = wide_int<64>(2);
const wide_int<64> constant::Three = wide_int<64>(3);

} // namespace grill
