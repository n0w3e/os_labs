#include "../include/timsort.h"
#include <pthread.h>
#include <vector>
#include <cstring>
#include <algorithm>
#include <climits>

struct ThreadArgs {
    int* array;
    size_t start;
    size_t end;
};

void* timsort_thread(void* args) {
    ThreadArgs* threadArgs = static_cast<ThreadArgs*>(args);
    std::sort(threadArgs->array + threadArgs->start, threadArgs->array + threadArgs->end);
    pthread_exit(nullptr);
    return nullptr;
}

void timsort(int* array, size_t size) {
    std::sort(array, array + size);
}

void multithreaded_timsort(int* array, size_t size, int num_threads) {
    size_t chunk_size = size / num_threads;
    pthread_t threads[num_threads];
    ThreadArgs threadArgs[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        threadArgs[i] = {array, i * chunk_size, (i == num_threads - 1) ? size : (i + 1) * chunk_size};
        pthread_create(&threads[i], nullptr, timsort_thread, &threadArgs[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    merge_sorted_chunks(array, size, chunk_size, num_threads);
}

void merge_sorted_chunks(int* array, size_t size, size_t chunk_size, int num_chunks) {
    std::vector<int> temp(size);
    size_t* indices = new size_t[num_chunks];
    memset(indices, 0, num_chunks * sizeof(size_t));

    for (size_t i = 0; i < size; ++i) {
        int min_index = -1;
        int min_value = INT_MAX;
        for (int j = 0; j < num_chunks; ++j) {
            if (indices[j] < chunk_size && (j * chunk_size + indices[j]) < size) {
                int value = array[j * chunk_size + indices[j]];
                if (value < min_value) {
                    min_value = value;
                    min_index = j;
                }
            }
        }
        temp[i] = min_value;
        indices[min_index]++;
    }

    std::copy(temp.begin(), temp.end(), array);
    delete[] indices;
}

void timsort_wrapper(int* array, size_t size, int /*num_threads*/) {
    timsort(array, size);
}