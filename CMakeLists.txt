cmake_minimum_required(VERSION 3.0.0)
project(HW2 VERSION 0.1.0)

add_subdirectory(lib/Calculator)

add_executable(HW2 src/main.cpp)

target_link_libraries(HW2 Calculator)
