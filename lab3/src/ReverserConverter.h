#pragma once

#include "Converter.h"
#include "ConverterFactory.h"
#include <vector>
#include <string>

class ReverserConverter : public IConverter {
private:
    int startSample;
    int endSample;

public:
    ReverserConverter() : startSample(0), endSample(0) {}

    void process(std::vector<int16_t>& buffer, size_t positionInSamples) override;
    void setParameters(const std::vector<std::string>& params) override;

    static IConverter* create() {
        return new ReverserConverter();
    }

    static std::string getDescription() {
        return "Reverses audio in the specified time interval";
    }

    static std::string getUsage() {
        return "reverse <start_seconds> <end_seconds>";
    }

    static std::string getName() {
        return "reverse";
    }
};