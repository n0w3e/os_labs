#include "my_math.h"
#include <cmath>
#include <stdexcept>

float E(int x) {
    if (x <= 0) {
        throw std::invalid_argument("x must be a positive integer.");
    }
    return pow(1.0 + 1.0 / x, x);
}

float Square(float A, float B) {
    if (A <= 0 || B <= 0) {
        throw std::invalid_argument("Sides must be positive.");
    }
    return A * B;
}
