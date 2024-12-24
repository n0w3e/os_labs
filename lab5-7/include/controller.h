#pragma once
#include "utils.h"
#include <thread>
#include <atomic>
#include <unordered_map>
#include <chrono>

class Controller {
private:
    std::map<int, ChildInfo> workers;
    std::atomic<bool> heartbitRunning;
    std::thread heartbitThread;
    std::unordered_map<int, std::chrono::time_point<std::chrono::steady_clock>> lastHeartbit;

    void processCommand(const std::string &command);
    void runHeartbit(int time);
public:
    Controller() : heartbitRunning(false) {}
    void run();
};