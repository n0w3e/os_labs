#ifndef TIMSORT_H
#define TIMSORT_H

#include <cstddef>

void TimSort(int* array, size_t size);
void MultithreadedTimsort(int* array, size_t size, int num_threads);
void MergeSortedChunks(int* array, size_t size, size_t chunk_size, int num_chunks);
void TimsortWrapper(int* array, size_t size, int num_threads);

#endif
