#ifndef TIMSORT_H
#define TIMSORT_H

#include <vector>
#include <thread>
#include <mutex>

void timSort(std::vector<int>& array, int maxThreads);
bool isSorted(const std::vector<int>& array);

#endif