#include "../include/worker_node.h"
#include <thread>
#include <chrono>
#include <iostream>

WorkerNode::WorkerNode(int id, const std::string &address) : id(id), address(address) {
    context = zmq::context_t(1);
    socket = zmq::socket_t(context, ZMQ_REP);
    socket.bind(address);

    std::thread heartbitThread(&WorkerNode::sendHeartbit, this, 2000);
    heartbitThread.detach();
}

void WorkerNode::sendHeartbit(int time) {
    while (true) {
        zmq::pollitem_t items[] = {{socket, 0, ZMQ_POLLOUT, 0}};
        zmq::poll(&items[0], 1, std::chrono::milliseconds(time));

        if (items[0].revents & ZMQ_POLLOUT) {
            std::string heartbitMessage = "heartbit id:" + std::to_string(id);
            zmq::message_t message(heartbitMessage.begin(), heartbitMessage.end());
            socket.send(message, zmq::send_flags::none);
            std::cout << "Worker " << id << " sent heartbit" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
}

void WorkerNode::processCommand(const std::string &command) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(command);
    while (std::getline(tokenStream, token, ' ')) {
        tokens.push_back(token);
    }

    if (tokens[0] == "exec") {
        if (tokens.size() == 3) {
            std::string key = tokens[2];
            if (localDict.find(key) != localDict.end()) {
                sendResponse(socket, "Ok:" + std::to_string(id) + ": " + std::to_string(localDict[key]));
            } else {
                sendResponse(socket, "Ok:" + std::to_string(id) + ": 'MyVar' not found");
            }
        } else if (tokens.size() == 4) {
            std::string key = tokens[2];
            int value = std::stoi(tokens[3]);
            localDict[key] = value;
            sendResponse(socket, "Ok:" + std::to_string(id));
        }
    } else if (tokens[0] == "ping") {
        sendResponse(socket, "Ok");
    }
}

void WorkerNode::run() {
    while (true) {
        std::string command = receiveRequest(socket);
        processCommand(command);
    }
}

WorkerNode::~WorkerNode() {
    socket.unbind(address);
}