project(OSMAI_Tests)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

# add_executable(lab1_test lab1_tests.cpp)
# target_sources(lab1_test PRIVATE ../lab1/src/child.c ../lab1/src/parent.c)
# target_include_directories(lab1_test PRIVATE ../lab1/include)
# target_link_libraries(lab1_test PRIVATE GTest::gtest GTest::gtest_main Threads::Threads)

add_executable(lab2_test lab2_tests.cpp)
target_sources(lab2_test PRIVATE ../lab2/src/timsort.cpp)
target_include_directories(lab2_test PRIVATE ../lab2/include)
target_link_libraries(lab2_test PRIVATE GTest::gtest GTest::gtest_main Threads::Threads)

enable_testing()
