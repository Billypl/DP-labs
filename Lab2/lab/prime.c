#include "NWD.h"
#include <stdbool.h>

bool is_prime(int a) {
    if (a <= 1) {
        return false;
    }

    for (int i = 2; i * i <= a; i++) {
        if (a % i == 0) {
            return false;
        }
    }

    return true;
}
