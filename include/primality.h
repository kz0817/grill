#pragma once
#include "integer.h"

class primality {
public:
    template<typename T>
    static bool fermat_test(const T& n) {
        const T one = 1;
        for (int i = 2; i <= 2; i++) {
            const T a = i;
            // TODO: implement
            // if (gcd(a, n) != one)
            //     continue;
            const bool mod_is_one = fermat_test(n, a, one);
            if (!mod_is_one)
                return false;
        }
        return true;
    }

private:
    static bool fermat_test(const integer& n, const integer& a, const integer & one) {
        return ((a.pow(n - one) % n) == one);
    }

};
