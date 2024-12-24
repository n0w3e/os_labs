cmake_minimum_required(VERSION 3.10)
project(DistributedSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZeroMQ REQUIRED libzmq)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

set(SOURCES
    src/controller.cpp
    src/worker_node.cpp
    src/utils.cpp
    main.cpp
)

add_executable(controller src/controller.cpp src/worker_node.cpp src/utils.cpp main.cpp)
target_include_directories(controller PRIVATE zmq)
target_link_libraries(controller PRIVATE zmq)

add_executable(worker src/worker.cpp src/worker_node.cpp src/utils.cpp)
target_include_directories(worker PRIVATE zmq)
target_link_libraries(worker PRIVATE zmq)