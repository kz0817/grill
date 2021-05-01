#pragma once
#include <memory>
#include "integer.h"

class primality {
    primality();
    virtual ~primality();

    static bool is_prime(const integer& n);

private:
    struct context;
    std::unique_ptr<context> ctx;
};
