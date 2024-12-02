cmake_minimum_required(VERSION 3.10)
project(lab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

add_library(timsort STATIC src/timsort.cpp)
target_include_directories(timsort PRIVATE include)

add_executable(lab2 main.cpp)
target_include_directories(lab2 PRIVATE include)
target_link_libraries(lab2 PRIVATE timsort Threads::Threads)
