#include "my_math.h"
#include <cmath>
#include <stdexcept>

float E(int x) {
    if (x < 0) {
        throw std::invalid_argument("x must be a non-negative integer.");
    }
    float result = 0.0;
    float factorial = 1.0;
    for (int n = 0; n <= x; ++n) {
        if (n > 0) factorial *= n;
        result += 1.0 / factorial;
    }
    return result;
}

float Square(float A, float B) {
    if (A <= 0 || B <= 0) {
        throw std::invalid_argument("Sides must be positive.");
    }
    return (A * B) / 2.0;
}
