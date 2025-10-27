cmake_minimum_required(VERSION 3.15)
project(lab0_b)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(googletest)

add_executable(lab0_b main.cpp lab0.h
        writetocsv.cpp
        filereader.cpp
        wordcounter.cpp
)

add_executable(test_lab0 test_lab0.cpp lab0.h
        writetocsv.cpp
        filereader.cpp
        wordcounter.cpp
)
target_link_libraries(test_lab0 gtest_main)