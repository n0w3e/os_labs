#ifndef MY_MATH_H
#define MY_MATH_H

#ifdef _WIN32
    #ifdef BUILD_MY_MATH
        #define MY_MATH_API __declspec(dllexport)
    #else
        #define MY_MATH_API __declspec(dllimport)
    #endif
#else
    #define MY_MATH_API
#endif

extern "C" {
    MY_MATH_API float E(int x);
    MY_MATH_API float Square(float A, float B);
}

#endif
