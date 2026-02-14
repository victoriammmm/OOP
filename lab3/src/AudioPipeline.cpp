#include "AudioPipeline.h"
#include "Exceptions.h"
#include "ConverterRegistry.h"
#include <iostream>

AudioPipeline::AudioPipeline(const std::string& configFile, const std::vector<std::string>& inputFiles,
    const std::string& outputFile): configFile(configFile), inputFiles(inputFiles),
    outputFile(outputFile), processedSamples(0) {

    if (inputFiles.empty()) {
        throw ConfigException("No input files specified");
    }

}

void AudioPipeline::build() {
    ConfigParser parser(configFile);
    auto commands = parser.parse();

    ConverterFactory* factory = ConverterFactory::getInstance();

    for (const auto& cmd : commands) {
        ConverterInstance instance;

        instance.converter = factory->create(cmd.name);
        if (!instance.converter) {
            throw ConfigException("Cannot create converter: " + cmd.name);
        }

        instance.converter->setParameters(cmd.parameters);

        if (cmd.name == "mix" && !cmd.parameters.empty()) {
            std::string fileRef = cmd.parameters[0];

            if (fileRef.empty() || fileRef[0] != '$') {
                throw ConfigException("Invalid file reference for mix: " + fileRef);
            }

            try {
                int fileIndex = std::stoi(fileRef.substr(1)) - 1;
                if (fileIndex < 0 || fileIndex >= static_cast<int>(inputFiles.size())) {
                    throw ConfigException("Invalid file index: " + fileRef +
                                        " (only " + std::to_string(inputFiles.size()) +
                                        " input files available)");
                }

                loadAdditionalStream(fileIndex, instance.additionalStream);
                instance.converter->setAdditionalData(instance.additionalStream);

            } catch (const std::invalid_argument&) {
                throw ConfigException("Invalid number in file reference: " + fileRef);
            } catch (const std::out_of_range&) {
                throw ConfigException("File index out of range: " + fileRef);
            }
        }

        converters.push_back(std::move(instance));
    }
}

void AudioPipeline::loadAdditionalStream(int fileIndex, std::vector<int16_t>& stream) {
    if (fileIndex < 0 || fileIndex >= static_cast<int>(inputFiles.size())) {
        throw ConfigException("Invalid file index: " + std::to_string(fileIndex + 1));
    }

    try {
        WavHandler wav(inputFiles[fileIndex]);
        const size_t BUFFER_SIZE = 44100;
        std::vector<int16_t> buffer(BUFFER_SIZE);

        stream.clear();

        while (true) {
            bool success = wav.readSamples(buffer, BUFFER_SIZE);
            if (!success || buffer.empty()) {
                break;
            }
            stream.insert(stream.end(), buffer.begin(), buffer.end());
        }

    } catch (const std::exception& e) {
        throw ConfigException("Cannot load additional stream from " +
                             inputFiles[fileIndex] + ": " + e.what());
    }
}

void AudioPipeline::process() {
    input = std::make_unique<WavHandler>(inputFiles[0]);

    if (input->getTotalSamples() == 0) {
        throw WavException("Input file is empty: " + inputFiles[0]);
    }

    output = std::make_unique<WavHandler>(outputFile, *input);

    const size_t BUFFER_SIZE = 44100;
    std::vector<int16_t> buffer(BUFFER_SIZE);
    size_t positionInSamples = 0;

    while (true) {
        bool success = input->readSamples(buffer, BUFFER_SIZE);
        if (!success || buffer.empty()) {
            break;
        }

        for (auto& instance : converters) {
            instance.converter->process(buffer, positionInSamples);
        }

        output->writeSamples(buffer);
        processedSamples += buffer.size();
        positionInSamples += buffer.size();
    }
    std::cout << "Output saved to: " << outputFile << "\n";

    input.reset();
    output.reset();
}