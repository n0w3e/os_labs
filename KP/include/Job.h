#ifndef JOB_H
#define JOB_H

#include <string>
#include <vector>
#include <mutex>

class Job {
public:
    std::string id;
    std::vector<std::string> dependencies;
    bool completed = false;
    bool failed = false;

    Job() = default;

    Job(const std::string& id, const std::vector<std::string>& dependencies);

    void execute();

private:
    static std::mutex output_mutex;
};

#endif