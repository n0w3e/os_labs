#include <gtest/gtest.h>
#include <dlfcn.h>
#include <stdexcept>
#include <cmath>

void* load_library(const std::string& library_path, float (**E)(int), float (**Square)(float, float)) {
    void* handle = dlopen(library_path.c_str(), RTLD_LAZY);
    if (!handle) {
        throw std::runtime_error(dlerror());
    }

    *E = (float (*)(int))dlsym(handle, "E");
    *Square = (float (*)(float, float))dlsym(handle, "Square");

    if (!*E || !*Square) {
        dlclose(handle);
        throw std::runtime_error(dlerror());
    }

    return handle;
}

TEST(MyMathTest, E_Impl1) {
    float (*E)(int);
    float (*Square)(float, float);
    void* handle = load_library("../lab4/libmy_math.so", &E, &Square);

    float result = E(10);

    EXPECT_NEAR(result, 2.59374, 0.00001);

    dlclose(handle);
}

TEST(MyMathTest, Square_Impl1) {
    float (*E)(int);
    float (*Square)(float, float);
    void* handle = load_library("../lab4/libmy_math.so", &E, &Square);

    float result = Square(5.0, 10.0);

    EXPECT_FLOAT_EQ(result, 50.0);

    dlclose(handle);
}

TEST(MyMathAlternativeTest, E_Impl2) {
    float (*E)(int);
    float (*Square)(float, float);
    void* handle = load_library("../lab4/libmy_math_alternative.so", &E, &Square);

    float result = E(10);

    EXPECT_NEAR(result, 2.71828, 0.00001);

    dlclose(handle);
}

TEST(MyMathAlternativeTest, Square_Impl2) {
    float (*E)(int);
    float (*Square)(float, float);
    void* handle = load_library("../lab4/libmy_math_alternative.so", &E, &Square);

    float result = Square(5.0, 10.0);

    EXPECT_FLOAT_EQ(result, 25.0);

    dlclose(handle);
}