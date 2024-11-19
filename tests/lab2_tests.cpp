#include <gtest/gtest.h>
#include <vector>
#include <thread>
#include "../lab2/include/timsort.h"

TEST(TimSortTests, SortsCorrectly) {
    std::vector<int> array = {34, 7, 23, 32, 5, 62, 32, 12, 4, 23};
    std::vector<int> expected = {4, 5, 7, 12, 23, 23, 32, 32, 34, 62};
    int maxThreads = 4;

    timSort(array, maxThreads);

    EXPECT_EQ(array, expected);
}

TEST(TimSortTests, HandlesEmptyArray) {
    std::vector<int> array;
    std::vector<int> expected;
    int maxThreads = 2;

    timSort(array, maxThreads);

    EXPECT_EQ(array, expected);
}

TEST(TimSortTests, HandlesSingleElement) {
    std::vector<int> array = {42};
    std::vector<int> expected = {42};
    int maxThreads = 2;

    timSort(array, maxThreads);

    EXPECT_EQ(array, expected);
}

TEST(TimSortTests, HandlesDuplicateElements) {
    std::vector<int> array = {5, 5, 5, 5, 5};
    std::vector<int> expected = {5, 5, 5, 5, 5};
    int maxThreads = 3;

    timSort(array, maxThreads);

    EXPECT_EQ(array, expected);
}

TEST(TimSortTests, RespectsThreadLimit) {
    std::vector<int> array = {8, 6, 7, 5, 3, 0, 9, 4, 2, 1};
    int maxThreads = 2;

    timSort(array, maxThreads);

    EXPECT_TRUE(std::thread::hardware_concurrency() >= maxThreads);
}

TEST(TimSortTests, HandlesAlreadySortedArray) {
    std::vector<int> array = {1, 2, 3, 4, 5};
    std::vector<int> expected = {1, 2, 3, 4, 5};
    int maxThreads = 4;

    timSort(array, maxThreads);

    EXPECT_EQ(array, expected);
}
