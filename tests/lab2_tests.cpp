#include "timsort.h"
#include <gtest/gtest.h>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cstring>
#include <iostream>
#include <sys/time.h>

double measure_time(void (*sort_func)(int*, size_t, int), int* array, size_t size, int num_threads) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    sort_func(array, size, num_threads);
    gettimeofday(&end, NULL);

    double start_time = start.tv_sec + start.tv_usec / 1000000.0;
    double end_time = end.tv_sec + end.tv_usec / 1000000.0;

    return end_time - start_time;
}

TEST(TimSortTests, SingleThreadCorrectness) {
    constexpr size_t size = 1000;
    std::vector<int> data(size);

    for (size_t i = 0; i < size; ++i) {
        data[i] = rand() % 1000;
    }

    std::vector<int> expected(data);
    std::sort(expected.begin(), expected.end());

    TimSort(data.data(), size);

    EXPECT_EQ(data, expected);
}

TEST(TimSortTests, SingleVsMultithreadedConsistency) {
    constexpr size_t size = 1000;
    int* data = new int[size];
    for (size_t i = 0; i < size; ++i) {
        data[i] = rand() % 10000;
    }

    int* single_thread_result = new int[size];
    int* multi_thread_result = new int[size];
    memcpy(single_thread_result, data, size * sizeof(int));
    memcpy(multi_thread_result, data, size * sizeof(int));

    TimSort(single_thread_result, size);
    MultithreadedTimsort(multi_thread_result, size, 4);

    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(single_thread_result[i], multi_thread_result[i]);
    }

    delete[] data;
    delete[] single_thread_result;
    delete[] multi_thread_result;
}

TEST(TimSortTests, MultithreadedPerformance) {
    constexpr size_t size = 1000000;
    int single_thread_data[size];
    int multi_thread_data[size];

    for (size_t i = 0; i < size; ++i) {
        single_thread_data[i] = rand();
        multi_thread_data[i] = single_thread_data[i];
    }

    constexpr int num_threads = 4;

    double single_time = measure_time(TimsortWrapper, single_thread_data, size, 1);
    std::cout << "Single-threaded time: " << single_time << " seconds\n";

    double multi_time = measure_time(MultithreadedTimsort, multi_thread_data, size, num_threads);
    std::cout << "Multi-threaded time: " << multi_time << " seconds\n";

    EXPECT_LT(multi_time, single_time);
}
