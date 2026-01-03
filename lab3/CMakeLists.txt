cmake_minimum_required(VERSION 3.15)
project(sound_processor)

enable_testing()

add_subdirectory(googletest)

add_executable(lab3
        main.cpp
        Application.cpp
        AudioPipeline.cpp
        ConfigParser.cpp
        ConverterRegistry.cpp
        WavHandler.cpp
        MuteConverter.cpp
        MixConverter.cpp
        ReverserConverter.cpp
)

add_executable(tests
        tests/TestAudioPipeline.cpp
        tests/TestApplication.cpp
        tests/TestConfigParser.cpp
        tests/TestConverters.cpp
        tests/TestFactory.cpp
        tests/TestWavHandler.cpp
)

target_sources(tests PRIVATE
        Application.cpp
        AudioPipeline.cpp
        ConfigParser.cpp
        ConverterRegistry.cpp
        WavHandler.cpp
        MuteConverter.cpp
        MixConverter.cpp
        ReverserConverter.cpp
)

target_include_directories(lab3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(tests gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)