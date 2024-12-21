#pragma once
#include <iostream>
#include <zmq.hpp>
#include <map>
#include <thread>
#include <unistd.h>
#include "utils.h"

class WorkerNode {
public:
    WorkerNode(int id, const std::string &address);
    ~WorkerNode();
    void run();

private:
    int id;
    zmq::context_t context;
    zmq::socket_t socket;
    std::string address;
    zmq::socket_t commandSocket;
    zmq::socket_t heartbitSocket;
    std::map<std::string, int> localDict;

    void processCommand(const std::string &command);
    void sendHeartbit(int time);
};