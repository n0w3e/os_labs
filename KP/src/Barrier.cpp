#include "../include/Barrier.h"

Barrier::Barrier(size_t count)
    : threshold(count), count(count), generation(0) {}

void Barrier::wait() {
    std::unique_lock<std::mutex> lock(mtx);
    auto gen = generation;

    if (--count == 0) {
        generation++;
        count = threshold;
        cv.notify_all();
    } else {
        cv.wait(lock, [this, gen] { return gen != generation; });
    }
}