#include "../include/Job.h"
#include <iostream>
#include <thread>

std::mutex Job::output_mutex;

Job::Job(const std::string& id, const std::vector<std::string>& dependencies)
    : id(id), dependencies(dependencies) {}

void Job::execute() {
    std::cout << "Executing job: " << id << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    completed = true;
}