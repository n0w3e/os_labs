project(lab8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(nlohmann_json)

set(SOURCES
    src/Job.cpp
    src/DAG.cpp
    src/ThreadPool.cpp
    src/Barrier.cpp
    main.cpp
)

include_directories(include)

add_executable(lab8 ${SOURCES})
target_link_libraries(lab8 nlohmann_json::nlohmann_json pthread )