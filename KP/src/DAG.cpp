#include "../include/DAG.h"
#include "../include/ThreadPool.h"
#include "../include/Barrier.h"
#include <iostream>
#include <queue>

DAG::DAG(const nlohmann::json& dag_json) {
    for (const auto& job_json : dag_json["jobs"]) {
        std::string id = job_json["id"];
        std::vector<std::string> dependencies = job_json["dependencies"].get<std::vector<std::string>>();
        jobs[id] = Job(id, dependencies);
    }
}

bool DAG::is_valid() const {
    for (const auto& [id, job] : jobs) {
        std::map<std::string, bool> visited;
        std::map<std::string, bool> rec_stack;
        if (has_cycle(id, visited, rec_stack)) {
            return false;
        }
    }

    if (!is_connected()) {
        return false;
    }

    if (!has_start_and_end_jobs()) {
        return false;
    }

    return true;
}

bool DAG::has_cycle(const std::string& job_id, std::map<std::string, bool>& visited, std::map<std::string, bool>& rec_stack) const {
    if (!visited[job_id]) {
        visited[job_id] = true;
        rec_stack[job_id] = true;

        for (const auto& dep : jobs.at(job_id).dependencies) {
            if (!visited[dep] && has_cycle(dep, visited, rec_stack)) {
                return true;
            } else if (rec_stack[dep]) {
                return true;
            }
        }
    }
    rec_stack[job_id] = false;
    return false;
}

bool DAG::is_connected() const {
    if (jobs.empty()) {
        return true;
    }

    std::map<std::string, bool> visited;

    for (const auto& [id, job] : jobs) {
        if (job.dependencies.empty()) {
            std::queue<std::string> queue;
            queue.push(id);
            visited[id] = true;

            while (!queue.empty()) {
                std::string current_job_id = queue.front();
                queue.pop();

                for (const auto& [other_id, other_job] : jobs) {
                    if (std::find(other_job.dependencies.begin(), other_job.dependencies.end(), current_job_id) != other_job.dependencies.end()) {
                        if (!visited[other_id]) {
                            queue.push(other_id);
                            visited[other_id] = true;
                        }
                    }
                }
            }
        }
    }

    for (const auto& [id, job] : jobs) {
        if (!visited[id]) {
            return false;
        }
    }

    return true;
}

bool DAG::has_start_and_end_jobs() const {
    bool has_start = false;
    bool has_end = false;

    for (const auto& [id, job] : jobs) {
        if (job.dependencies.empty()) {
            has_start = true;
        }

        bool is_end = true;
        for (const auto& [_, other_job] : jobs) {
            if (std::find(other_job.dependencies.begin(), other_job.dependencies.end(), id) != other_job.dependencies.end()) {
                is_end = false;
                break;
            }
        }
        if (is_end) {
            has_end = true;
        }
    }

    return has_start && has_end;
}

void DAG::execute(int max_concurrent_jobs) {
    ThreadPool pool(max_concurrent_jobs);
    Barrier barrier(jobs.size());

    for (auto& [id, job] : jobs) {
        pool.enqueue([&job, &barrier, this]() {
            for (const auto& dep : job.dependencies) {
                while (!jobs[dep].completed && !jobs[dep].failed) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                if (jobs[dep].failed) {
                    job.failed = true;
                    return;
                }
            }

            job.execute();

            barrier.wait();
        });
    }
}