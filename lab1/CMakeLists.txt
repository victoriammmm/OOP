cmake_minimum_required(VERSION 3.15)
project(lab1)

enable_testing()

add_subdirectory(googletest)

add_executable(lab1 tests.cpp bitarray.cpp)

target_link_libraries(lab1 gtest_main)