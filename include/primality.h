#pragma once
#include "integer.h"
#include "constant.h"
#include "util.h"

class primality {
public:
    static bool fermat_test(const integer& n);
};
