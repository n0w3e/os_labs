#include <iostream>
#include <zmq.hpp>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>
#include "../include/controller.h"

void Controller::runHeartbit(int time) {
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    for (const auto &worker : workers) {
        subscriber.connect(worker.second.address + "_pub");
        subscriber.set(zmq::sockopt::subscribe, "heartbit");
    }

    while (heartbitRunning) {
        auto now = std::chrono::steady_clock::now();
        for (const auto &worker : workers) {
            auto it = lastHeartbit.find(worker.first);
            if (it != lastHeartbit.end()) {
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second).count();
                if (elapsed > 4 * time) {
                    std::cout << "Heartbit: node " << worker.first << " is unavailable now" << std::endl;
                } else {
                    std::cout << "Heartbit: node " << worker.first << " is available" << std::endl;
                }
            }
        }

        zmq::message_t message;
        while (subscriber.recv(message, zmq::recv_flags::dontwait)) {
            std::string msg(static_cast<char*>(message.data()), message.size());
            if (msg.find("heartbit") != std::string::npos) {
                int id = std::stoi(msg.substr(msg.find("id:") + 3));
                lastHeartbit[id] = std::chrono::steady_clock::now();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
}

void Controller::processCommand(const std::string &command) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(command);
    while (std::getline(tokenStream, token, ' ')) {
        tokens.push_back(token);
    }

    if (tokens[0] == "create") {
        int id = std::stoi(tokens[1]);
        if (id == -1) {
            std::cout << "Error: Invalid id format" << std::endl;
            return;
        }
        if (workers.find(id) != workers.end()) {
            std::cout << "Error: Already exists" << std::endl;
            return;
        }
        ChildInfo info;
        createWorker(id, info);
        workers[id] = info;
        lastHeartbit[id] = std::chrono::steady_clock::now();
    } else if (tokens[0] == "exec") {
        int id = std::stoi(tokens[1]);
        if (workers.find(id) == workers.end()) {
            std::cout << "Error:" << id << ": Not found" << std::endl;
            return;
        }
        zmq::context_t context(1);
        zmq::socket_t workerSocket(context, ZMQ_REQ);
        workerSocket.set(zmq::sockopt::linger, 0);
        workerSocket.connect(workers[id].address);

        std::string command = tokens[0] + " " + tokens[1];
        for (size_t i = 2; i < tokens.size(); ++i) {
            command += " " + tokens[i];
        }

        std::string response;
        if (sendRequestWithTimeout(workerSocket, command, response, 1000)) {
            std::cout << response << std::endl;
            lastHeartbit[id] = std::chrono::steady_clock::now();
        } else {
            std::cout << "Error:" << id << ": Node is unavailable" << std::endl;
        }
        workerSocket.close();
    } else if (tokens[0] == "heartbit") {
        int time = std::stoi(tokens[1]);
        if (heartbitRunning) {
            std::cout << "Heartbit is already running. Use 'stop' to stop it." << std::endl;
            return;
        }
        heartbitRunning = true;
        std::cout << "Ok" << std::endl;
        heartbitThread = std::thread(&Controller::runHeartbit, this, time);
    } else if (tokens[0] == "stop") {
        if (!heartbitRunning) {
            std::cout << "Heartbit is not running." << std::endl;
            return;
        }
        heartbitRunning = false;
        heartbitThread.join();
        std::cout << "Heartbit stopped." << std::endl;
    } else if (tokens[0] == "ping") {
        if (tokens.size() < 2) {
            std::cout << "Error: Missing node ID" << std::endl;
            return;
        }
        int id = std::stoi(tokens[1]);
        auto it = lastHeartbit.find(id);
        if (it != lastHeartbit.end()) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second).count();
            if (elapsed <= 4 * 2000) {
                std::cout << "Ok: " << id << ": 1" << std::endl;
            } else {
                std::cout << "Ok: " << id << ": 0" << std::endl;
            }
        } else {
            std::cout << "Error: Node " << id << " not found" << std::endl;
        }
    } else {
        std::cout << "Error: Unknown command" << std::endl;
    }
}

void Controller::run() {
    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "exit") {
            break;
        }
        processCommand(command);
    }
}