#include "../include/timsort.h"
#include <algorithm>
#include <iostream>
#include <condition_variable>

std::mutex threadMutex;
std::condition_variable threadCondition;
int currentThreads = 0;

constexpr int MIN_RUN = 32;

void insertionSort(std::vector<int>& array, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int temp = array[i];
        int j = i - 1;
        while (j >= left && array[j] > temp) {
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = temp;
    }
}

void merge(std::vector<int>& array, int left, int mid, int right) {
    int len1 = mid - left + 1, len2 = right - mid;
    std::vector<int> leftArray(len1), rightArray(len2);

    std::copy(array.begin() + left, array.begin() + mid + 1, leftArray.begin());
    std::copy(array.begin() + mid + 1, array.begin() + right + 1, rightArray.begin());

    int i = 0, j = 0, k = left;
    while (i < len1 && j < len2) {
        if (leftArray[i] <= rightArray[j]) {
            array[k++] = leftArray[i++];
        } else {
            array[k++] = rightArray[j++];
        }
    }
    while (i < len1) array[k++] = leftArray[i++];
    while (j < len2) array[k++] = rightArray[j++];
}

void timSortChunk(std::vector<int>& array, int left, int right) {
    insertionSort(array, left, right);
}

void timSort(std::vector<int>& array, int maxThreads) {
    int n = array.size();
    std::vector<std::thread> threads;

    for (int i = 0; i < n; i += MIN_RUN) {
        int right = std::min(i + MIN_RUN - 1, n - 1);

        {
            std::unique_lock<std::mutex> lock(threadMutex);
            threadCondition.wait(lock, [&]() { return currentThreads < maxThreads; });
            ++currentThreads;
        }

        threads.emplace_back([&, i, right]() {
            timSortChunk(array, i, right);
            {
                std::lock_guard<std::mutex> lock(threadMutex);
                --currentThreads;
                threadCondition.notify_one();
            }
        });
    }

    for (auto& thread : threads) thread.join();

    for (int size = MIN_RUN; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, n - 1);

            if (mid < right) merge(array, left, mid, right);
        }
    }
}

bool isSorted(const std::vector<int>& array) {
    return std::is_sorted(array.begin(), array.end());
}
