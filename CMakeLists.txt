cmake_minimum_required(VERSION 3.2)
project(SimplePA)

set(CMAKE_CXX_STANDARD 17)

add_library(SimplePA SimplePA.cpp SimplePA.hpp)
target_link_libraries(SimplePA pulse pulse-simple)

add_executable(SimplePA_Test test.cpp)
target_link_libraries(SimplePA_Test SimplePA pthread)