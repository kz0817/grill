#pragma once
#include <memory>
#include "integer.h"

class primality {
public:
    static bool is_prime(const integer& n);
    static bool fermat_test(const integer& n);
};
