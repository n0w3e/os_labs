#include <iostream>
#include <dlfcn.h>
#include <stdexcept>

int main() {
    try {
        void* handle = dlopen("./libmy_math.so", RTLD_LAZY);
        if (!handle) {
            throw std::runtime_error(dlerror());
        }

        auto E = (float (*)(int))dlsym(handle, "E");
        auto Square = (float (*)(float, float))dlsym(handle, "Square");
        if (!E || !Square) {
            dlclose(handle);
            throw std::runtime_error(dlerror());
        }

        int choice;
        std::cout << "Choose an operation:\n"
                  << "1. Calculate e\n"
                  << "2. Calculate area\n";
        std::cin >> choice;

        if (choice == 1) {
            int x;
            std::cout << "Enter x: ";
            std::cin >> x;

            std::cout << "e: " << E(x) << "\n";
        } else if (choice == 2) {
            float A, B;
            std::cout << "Enter A and B: ";
            std::cin >> A >> B;

            std::cout << "Area: " << Square(A, B) << "\n";
        } else {
            std::cout << "Invalid choice.\n";
        }

        dlclose(handle);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}