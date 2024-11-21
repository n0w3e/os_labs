#ifndef TIMSORT_H
#define TIMSORT_H

#include <cstddef>

void timsort(int* array, size_t size);
void multithreaded_timsort(int* array, size_t size, int num_threads);
void merge_sorted_chunks(int* array, size_t size, size_t chunk_size, int num_chunks);
void timsort_wrapper(int* array, size_t size, int num_threads);

#endif
