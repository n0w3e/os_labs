project(OSMAI_Tests)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(nlohmann_json)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

# Lab1
# add_executable(lab1_test lab1_tests.cpp)
# target_sources(lab1_test PRIVATE ../lab1/src/child.c ../lab1/src/parent.c)
# target_include_directories(lab1_test PRIVATE ../lab1/include)
# target_link_libraries(lab1_test PRIVATE GTest::gtest GTest::gtest_main Threads::Threads)

# Lab2
# add_executable(lab2_test lab2_tests.cpp)
# target_sources(lab2_test PRIVATE ../lab2/src/timsort.cpp)
# target_include_directories(lab2_test PRIVATE ../lab2/include)
# target_link_libraries(lab2_test PRIVATE GTest::gtest GTest::gtest_main Threads::Threads)

# Lab3
# add_executable(lab3_test lab3_tests.cpp)
# target_sources(lab3_test PRIVATE ../lab3/src/parent.cpp ../lab3/src/child.cpp)
# target_include_directories(lab3_test PRIVATE ../lab3/include)
# target_link_libraries(lab3_test PRIVATE GTest::gtest GTest::gtest_main Threads::Threads rt)

# Lab4
# add_executable(lab4_tests lab4_tests.cpp)
# target_include_directories(lab4_tests PRIVATE ../lab4/include)
# target_link_libraries(lab4_tests PRIVATE GTest::gtest GTest::gtest_main Threads::Threads dl)

# Lab5
# add_executable(lab5_tests lab5_tests.cpp)
# target_include_directories(lab5_tests PRIVATE ../lab5/include)
# target_link_libraries(lab5_tests PRIVATE GTest::gtest GTest::gtest_main Threads::Threads dl /opt/homebrew/include)

# Lab8
add_executable(lab8_tests lab8_tests.cpp)
target_sources(lab8_tests PRIVATE ../lab8/src/Job.cpp ../lab8/src/DAG.cpp ../lab8/src/ThreadPool.cpp ../lab8/src/Barrier.cpp)
target_include_directories(lab8_tests PRIVATE ../lab8/include)
target_link_libraries(lab8_tests PRIVATE GTest::gtest GTest::gtest_main Threads::Threads nlohmann_json::nlohmann_json)

enable_testing()
