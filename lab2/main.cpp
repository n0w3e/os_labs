#include "../include/timsort.h"
#include <iostream>
#include <vector>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <max_threads>\n";
        return 1;
    }

    int maxThreads = std::stoi(argv[1]);

    std::vector<int> array = {34, 7, 23, 32, 5, 62, 32, 12, 4, 23};

    std::cout << "Original array:\n";
    for (int num : array) std::cout << num << " ";
    std::cout << "\n";

    auto start = std::chrono::high_resolution_clock::now();
    timSort(array, maxThreads);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Sorted array:\n";
    for (int num : array) std::cout << num << " ";
    std::cout << "\n";

    std::cout << "Time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() 
              << " ms\n";

    return 0;
}
