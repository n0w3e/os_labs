#include "timsort.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

void print_array(const int* array, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    size_t size;
    int num_threads;

    std::cout << "Enter the size of the array: ";
    std::cin >> size;
    std::cout << "Enter the number of threads: ";
    std::cin >> num_threads;

    std::vector<int> array(size);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (size_t i = 0; i < size; ++i) {
        array[i] = std::rand() % 100;
    }

    std::cout << "Original array: ";
    print_array(array.data(), size);

    multithreaded_timsort(array.data(), size, num_threads);

    std::cout << "Sorted array: ";
    print_array(array.data(), size);

    return 0;
}
