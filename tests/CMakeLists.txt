project(OSMAI)
include(FetchContent)

set(CMAKE_CXX_STANDARD 11)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

add_executable(lab1_test lab1_tests.cpp)
target_sources(lab1_test PRIVATE ../lab1/src/child.c ../lab1/src/parent.c)
target_include_directories(lab1_test PRIVATE ../lab1/include)
target_link_libraries(lab1_test PRIVATE GTest::gtest GTest::gtest_main)

add_test(lab1_test lab1_test)
