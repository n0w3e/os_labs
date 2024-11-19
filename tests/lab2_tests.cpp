#include <gtest/gtest.h>
#include <vector>
#include <chrono>
#include <algorithm>
#include "../lab2/include/timsort.h"

bool is_sorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

TEST(TimSortBasicTests, SmallArraySort) {
    std::vector<int> data = {5, 2, 9, 1, 5, 6, 7};
    std::vector<int> expected = data;
    std::sort(expected.begin(), expected.end());

    timSort(data, 4);

    ASSERT_EQ(data, expected);
    ASSERT_TRUE(is_sorted(data));
}


TEST(TimSortBasicTests, DuplicatesArraySort) {
    std::vector<int> data = {4, 4, 2, 2, 3, 3, 1, 1};
    std::vector<int> expected = data;
    std::sort(expected.begin(), expected.end());

    timSort(data, 2);

    ASSERT_EQ(data, expected);
    ASSERT_TRUE(is_sorted(data));
}

TEST(TimSortBasicTests, AlreadySortedArray) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> expected = data;

    timSort(data, 3);

    ASSERT_EQ(data, expected);
    ASSERT_TRUE(is_sorted(data));
}

TEST(TimSortNewTests, SingleThreadCorrectness) {
    std::vector<int> data = {5, 2, 9, 1, 5, 6, 7};
    std::vector<int> expected = data;
    std::sort(expected.begin(), expected.end());

    timSort(data, 1);

    ASSERT_EQ(data, expected);
    ASSERT_TRUE(is_sorted(data));
}

TEST(TimSortNewTests, SingleVsMultithreadedConsistency) {
    std::vector<int> data = {8, 3, 7, 1, 2, 9, 5, 4};
    std::vector<int> single_threaded_result = data;
    std::vector<int> multi_threaded_result = data;

    timSort(single_threaded_result, 1);
    timSort(multi_threaded_result, 4);

    ASSERT_EQ(single_threaded_result, multi_threaded_result);
    ASSERT_TRUE(is_sorted(single_threaded_result));
    ASSERT_TRUE(is_sorted(multi_threaded_result));
}

TEST(TimSortNewTests, MultithreadedPerformance) {
    constexpr size_t data_size = 1'000'000;
    std::vector<int> data(data_size);
    for (size_t i = 0; i < data_size; ++i) {
        data[i] = rand() % 100000;
    }

    std::vector<int> single_threaded_data = data;
    std::vector<int> multi_threaded_data = data;

    auto start_single = std::chrono::high_resolution_clock::now();
    timSort(single_threaded_data, 1);
    auto end_single = std::chrono::high_resolution_clock::now();

    auto start_multi = std::chrono::high_resolution_clock::now();
    timSort(multi_threaded_data, 8);
    auto end_multi = std::chrono::high_resolution_clock::now();

    auto single_threaded_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_single - start_single).count();
    auto multi_threaded_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_multi - start_multi).count();

    std::cout << "Single-threaded duration: " << single_threaded_duration << " ms\n";
    std::cout << "Multi-threaded duration: " << multi_threaded_duration << " ms\n";


    ASSERT_TRUE(is_sorted(single_threaded_data));
    ASSERT_TRUE(is_sorted(multi_threaded_data));
    ASSERT_LE(multi_threaded_duration, single_threaded_duration);
}
