#pragma once

#include <cmath>

namespace double_funcs {
    const double EPS = 0.00001;

    bool equal(double a, double b) {
        return std::abs(a - b) < EPS;
    }
}
