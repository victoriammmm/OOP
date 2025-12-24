cmake_minimum_required(VERSION 3.5.0)
project(lab2 VERSION 0.1.0 LANGUAGES C CXX)

enable_testing()
add_subdirectory(googletest)

add_executable(lab2 Controller.cpp Field.cpp SaveToFile.cpp main.cpp
        Universe.cpp Rule.cpp View.cpp LoadFromFile.cpp Universe.h)

add_executable(TestField tests/TestField.cpp Field.cpp)
target_link_libraries(TestField gtest gtest_main)
add_test(NAME FieldTests COMMAND TestField)

add_executable(TestRule tests/RuleTest.cpp Rule.cpp)
target_link_libraries(TestRule gtest gtest_main)
add_test(NAME RuleTests COMMAND TestRule)

add_executable(TestUniverse tests/UniverseTest.cpp Universe.cpp Field.cpp Rule.cpp)
target_link_libraries(TestUniverse gtest gtest_main)
add_test(NAME UniverseTests COMMAND TestUniverse)

add_executable(TestLoader tests/LoaderTest.cpp LoadFromFile.cpp Universe.cpp Field.cpp Rule.cpp)
target_link_libraries(TestLoader gtest gtest_main)
add_test(NAME LoaderTests COMMAND TestLoader)

include(CTest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)