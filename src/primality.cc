#include "primality.h"

struct primality::context {
};

primality::primality()
: ctx(new context()) {
}

primality::~primality() {
}

bool primality::is_prime(const integer &n) {
    // TODO: implement
    return true;
}
