#pragma once

#include <cmath>

namespace double_funcs {
    const double EPS = 0.0000001;

    inline bool equal(double a, double b) {
        return std::abs(a - b) < EPS;
    }
}
