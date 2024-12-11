set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(my_math SHARED src/impl1.cpp)
add_library(my_math_alternative SHARED src/impl2.cpp)

add_executable(lab4_static main.cpp)
target_link_libraries(lab4_static my_math dl)

add_executable(lab4_dynamic main_dynamic.cpp)
target_link_libraries(lab4_dynamic my_math dl)
