#include "Application.h"
#include "AudioPipeline.h"
#include "ConverterRegistry.h"
#include "Exceptions.h"
#include <iostream>
#include <cstring>

int Application::run(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: no arguments provided.\n";
        std::cerr << "Use: -h for help\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "-h") {
        if (argc != 2) {
            std::cerr << "Error: -h flag should be used alone\n";
            return 1;
        }
        printHelp();
        return 0;
    }

    if (mode != "-c") {
        std::cerr << "Error: unknown mode '" << mode << "'\n";
        std::cerr << "Use: -h for help\n";
        return 1;
    }

    if (argc < 5) {
        std::cerr << "Error: insufficient arguments for -c mode\n";
        std::cerr << "Use: -c <config> <output> <input1> [input2...]\n";
        return 1;
    }

    std::string configFile = argv[2];
    std::string outputFile = argv[3];
    std::vector<std::string> inputFiles;

    for (int i = 4; i < argc; ++i) {
        if (!isWavFile(argv[i])) {
            std::cerr << "Error: file must have .wav extension: " << argv[i] << "\n";
            return 1;
        }
        inputFiles.emplace_back(argv[i]);
    }

    try {
        AudioPipeline pipeline(configFile, inputFiles, outputFile);
        pipeline.build();
        pipeline.process();
        std::cout << "Success: processing completed.\n";
        return 0;
    } catch (const SoundProcessorException& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }
}

bool Application::isWavFile(const char* str) {
    if (!str) return false;
    size_t len = strlen(str);
    return (len > 4 && strcmp(str + len - 4, ".wav") == 0);
}

void Application::printHelp() const {
    std::cout << "Usage:\n";
    std::cout << "-c <config.txt> <output.wav> <input1.wav> [input2.wav ...]\n\n";

    std::cout << "Options:\n";
    std::cout << "-h: show this help message\n";
    std::cout << "-c: process audio with configuration file\n\n";

    std::cout << "Available converters:\n";

    for (const auto& name : ConverterRegistry::getAvailableConverters()) {
        std::cout << "  " << ConverterRegistry::getUsage(name) << "\n";
        std::cout << "    " << ConverterRegistry::getDescription(name) << "\n\n";
    }
}