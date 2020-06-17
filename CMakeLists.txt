cmake_minimum_required(VERSION 3.12)
project(2031611)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(InputReader)

add_executable(2031611 main.cpp InputData.cpp InputData.hpp Date.cpp Date.hpp Investment.cpp Investment.hpp)

target_link_libraries(2031611 PRIVATE InputReader)