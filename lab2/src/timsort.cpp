#include "../include/timsort.h"
#include <pthread.h>
#include <vector>
#include <cstring>
#include <algorithm>
#include <climits>

constexpr size_t RUN = 32;

struct ThreadArgs {
    int* array;
    size_t start;
    size_t end;
};

void* TimsortThread(void* args) {
    ThreadArgs* threadArgs = static_cast<ThreadArgs*>(args);
    std::sort(threadArgs->array + threadArgs->start, threadArgs->array + threadArgs->end);
    pthread_exit(nullptr);
    return nullptr;
}

void InsertionSort(int* array, size_t left, size_t right) {
    for (size_t i = left + 1; i <= right; ++i) {
        int temp = array[i];
        size_t j = i;
        while (j > left && array[j - 1] > temp) {
            array[j] = array[j - 1];
            --j;
        }
        array[j] = temp;
    }
}

void Merge(int* array, size_t left, size_t mid, size_t right) {
    size_t len1 = mid - left + 1;
    size_t len2 = right - mid;

    std::vector<int> left_part(len1);
    std::vector<int> right_part(len2);

    std::memcpy(left_part.data(), &array[left], len1 * sizeof(int));
    std::memcpy(right_part.data(), &array[mid + 1], len2 * sizeof(int));

    size_t i = 0, j = 0, k = left;

    while (i < len1 && j < len2) {
        if (left_part[i] <= right_part[j]) {
            array[k++] = left_part[i++];
        } else {
            array[k++] = right_part[j++];
        }
    }

    while (i < len1) {
        array[k++] = left_part[i++];
    }
    while (j < len2) {
        array[k++] = right_part[j++];
    }
}

void TimSort(int* array, size_t size) {
    for (size_t i = 0; i < size; i += RUN) {
        size_t right = std::min(i + RUN - 1, size - 1);
        InsertionSort(array, i, right);
    }

    for (size_t run_size = RUN; run_size < size; run_size *= 2) {
        for (size_t left = 0; left < size; left += 2 * run_size) {
            size_t mid = left + run_size - 1;
            size_t right = std::min(left + 2 * run_size - 1, size - 1);

            if (mid < right) {
                Merge(array, left, mid, right);
            }
        }
    }
}

void MultithreadedTimsort(int* array, size_t size, int num_threads) {
    size_t chunk_size = size / num_threads;
    pthread_t threads[num_threads];
    ThreadArgs threadArgs[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        threadArgs[i] = {array, i * chunk_size, (i == num_threads - 1) ? size : (i + 1) * chunk_size};
        pthread_create(&threads[i], nullptr, TimsortThread, &threadArgs[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    MergeSortedChunks(array, size, chunk_size, num_threads);
}

void MergeSortedChunks(int* array, size_t size, size_t chunk_size, int num_chunks) {
    std::vector<int> temp(size);
    std::vector<size_t> indices(num_chunks, 0);

    for (size_t i = 0; i < size; ++i) {
        int min_index = -1;
        int min_value = INT_MAX;

        for (int chunk = 0; chunk < num_chunks; ++chunk) {
            size_t chunk_start = chunk * chunk_size;
            size_t chunk_end = std::min(chunk_start + chunk_size, size);

            if (indices[chunk] < chunk_end - chunk_start) {
                int value = array[chunk_start + indices[chunk]];
                if (value < min_value) {
                    min_value = value;
                    min_index = chunk;
                }
            }
        }

        temp[i] = min_value;
        ++indices[min_index];
    }

    std::copy(temp.begin(), temp.end(), array);
}


void TimsortWrapper(int* array, size_t size, int /*num_threads*/) {
    TimSort(array, size);
}