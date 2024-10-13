project(OSMAI)
include(FetchContent)

set(CMAKE_CXX_STANDARD 11)

# Загружаем GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

# Создаем исполняемый файл для тестов
add_executable(lab1_test lab1_tests.cpp)

# Указываем исходники из основного проекта, чтобы была доступна реализация child_process
target_sources(lab1_test PRIVATE ../lab1/src/child.c ../lab1/src/parent.c)

# Указываем директорию с заголовочными файлами
target_include_directories(lab1_test PRIVATE ../lab1/include)

# Привязываем библиотеки GoogleTest к тестам
target_link_libraries(lab1_test PRIVATE GTest::gtest GTest::gtest_main)

# Добавляем тесты
add_test(lab1_test lab1_test)
