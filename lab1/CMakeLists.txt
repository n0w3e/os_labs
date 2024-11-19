set(CMAKE_C_STANDARD 11)
project(lab1)

add_executable(lab1 src/child.c src/parent.c main.c)
target_include_directories(lab1 PRIVATE include)
