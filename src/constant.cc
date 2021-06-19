#include "Integer.h"
#include "constant.h"

namespace grill {

const WideInt<64> constant::Zero = WideInt<64>(static_cast<Integer::block_t>(0));
const WideInt<64> constant::One = WideInt<64>(1);
const WideInt<64> constant::Two = WideInt<64>(2);
const WideInt<64> constant::Three = WideInt<64>(3);

} // namespace grill
