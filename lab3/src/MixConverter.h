#pragma once

#include "Converter.h"
#include "ConverterFactory.h"
#include <vector>
#include <string>

class MixConverter : public IConverter {
private:
    int startSample;
    std::vector<int16_t> additionalStream;

public:
    MixConverter() : startSample(0) {}

    void setParameters(const std::vector<std::string>& params) override;
    void process(std::vector<int16_t>& buffer, size_t positionInSamples) override;
    void setAdditionalData(const std::vector<int16_t>& data) override {
        additionalStream = data;
    }

    static IConverter* create() {
        return new MixConverter();
    }

    static std::string getDescription() {
        return "Mixes main stream with additional stream starting from specified time";
    }

    static std::string getUsage() {
        return "mix $<file_number> <start_seconds>";
    }

    static std::string getName() {
        return "mix";
    }
};