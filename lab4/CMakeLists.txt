cmake_minimum_required(VERSION 4.0)
project(oop_lab4)

set(CMAKE_CXX_STANDARD 17)


add_executable(oop_lab4 main.cpp CSVParser.h TuplePrinter.h)
target_include_directories(oop_lab4 PRIVATE ${CMAKE_SOURCE_DIR}/src)

include(FetchContent)
FetchContent_Declare(
googletest
DOWNLOAD_EXTRACT_TIMESTAMP TRUE
URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_executable(test_tuple_printer
TestTuplePrinter.cpp
TestCSVParser.cpp
TestImprovedCSVParser.cpp
)

target_include_directories(test_tuple_printer PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(test_tuple_printer PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(test_tuple_printer)