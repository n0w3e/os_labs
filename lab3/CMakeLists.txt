set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(lab3)

add_executable(lab3 src/child.cpp src/parent.cpp main.cpp)
target_include_directories(lab3 PRIVATE include)
target_link_libraries(lab3 PRIVATE rt)
