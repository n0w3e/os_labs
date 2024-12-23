#ifndef BARRIER_H
#define BARRIER_H

#include <mutex>
#include <condition_variable>

class Barrier {
public:
    explicit Barrier(size_t count);
    void wait();

private:
    std::mutex mtx;
    std::condition_variable cv;
    size_t threshold;
    size_t count;
    size_t generation;
};

#endif