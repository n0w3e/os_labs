#include <gtest/gtest.h>
#include <zmq.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include "../lab5/include/controller.h"
#include "../lab5/include/worker_node.h"
#include "../lab5/include/utils.h"

TEST(ControllerTest, ProcessCreateCommand) {
    Controller controller;
    std::string command = "create 1";
    int initialWorkerCount = controller.getWorkerCount();

    controller.executeCommand(command);

    EXPECT_EQ(controller.getWorkerCount(), initialWorkerCount + 1);
}

TEST(ControllerTest, ProcessExecCommand) {
    Controller controller;
    std::string createCommand = "create 1";
    controller.executeCommand(createCommand);

    std::string execCommand = "exec 1 set MyVar 42";
    zmq::context_t context(1);
    zmq::socket_t workerSocket(context, ZMQ_REQ);
    workerSocket.set(zmq::sockopt::linger, 0);
    workerSocket.connect(controller.getWorkerAddress(1));

    std::string response;
    bool result = sendRequestWithTimeout(workerSocket, execCommand, response, 1000);

    EXPECT_TRUE(result);
    EXPECT_EQ(response, "Ok:1");
    workerSocket.close();
}

TEST(ControllerTest, ProcessPingCommand) {
    Controller controller;
    std::string createCommand = "create 1";
    controller.executeCommand(createCommand);

    std::string pingCommand = "ping 1";

    std::stringstream output;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());
    controller.executeCommand(pingCommand);
    std::cout.rdbuf(oldCoutStreamBuf);

    EXPECT_EQ(output.str(), "Ok: 1: 1\n");
}

TEST(WorkerNodeTest, ProcessExecCommand) {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("inproc://test_worker");

    WorkerNode worker(1, "inproc://test_worker");
    std::string command = "exec set MyVar 42";

    worker.executeCommand(command);

    EXPECT_EQ(worker.getLocalDictValue("MyVar"), 42);
    socket.close();
}

TEST(WorkerNodeTest, ProcessPingCommand) {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("inproc://test_worker");

    WorkerNode worker(1, "inproc://test_worker");
    std::string command = "ping";

    worker.executeCommand(command);

    EXPECT_NO_THROW(worker.executeCommand(command));
    socket.close();
}

TEST(UtilsTest, SendRequestWithTimeoutSuccess) {
    zmq::context_t context(1);
    zmq::socket_t serverSocket(context, ZMQ_REP);
    serverSocket.bind("inproc://test_server");

    zmq::socket_t clientSocket(context, ZMQ_REQ);
    clientSocket.connect("inproc://test_server");

    std::string request = "test request";
    std::string response = "test response";

    std::thread serverThread([&serverSocket, &response]() {
        zmq::message_t req;
        serverSocket.recv(req, zmq::recv_flags::none);
        zmq::message_t reply(response.size());
        memcpy(reply.data(), response.c_str(), response.size());
        serverSocket.send(reply, zmq::send_flags::none);
    });

    std::string receivedResponse;
    bool result = sendRequestWithTimeout(clientSocket, request, receivedResponse, 1000);

    EXPECT_TRUE(result);
    EXPECT_EQ(receivedResponse, response);

    serverThread.join();
    serverSocket.close();
    clientSocket.close();
}

TEST(UtilsTest, SendRequestWithTimeoutTimeout) {
    zmq::context_t context(1);
    zmq::socket_t clientSocket(context, ZMQ_REQ);
    clientSocket.connect("inproc://test_server");

    std::string request = "test request";

    std::string receivedResponse;
    bool result = sendRequestWithTimeout(clientSocket, request, receivedResponse, 100);

    EXPECT_FALSE(result);
    clientSocket.close();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}