#pragma once

#include "WavHandler.h"
#include "ConfigParser.h"
#include "ConverterFactory.h"
#include <memory>
#include <vector>
#include <string>

class AudioPipeline {
public:
    AudioPipeline(const std::string& configFile,
                  const std::vector<std::string>& inputFiles,
                  const std::string& outputFile);

    void build();
    void process();

    size_t getProcessedSamples() const {
        return processedSamples;
    }
    size_t getConverterCount() const {
        return converters.size();
    }

private:
    struct ConverterInstance {
        std::unique_ptr<IConverter> converter;
        std::vector<int16_t> additionalStream;
    };

    void loadAdditionalStream(int fileIndex, std::vector<int16_t>& stream);

    std::vector<std::string> inputFiles;
    std::string outputFile;
    std::string configFile;

    std::unique_ptr<WavHandler> input;
    std::unique_ptr<WavHandler> output;
    std::vector<ConverterInstance> converters;

    size_t processedSamples;
};