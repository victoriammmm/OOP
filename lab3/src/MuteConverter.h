#pragma once

#include "Converter.h"
#include "ConverterFactory.h"
#include <vector>
#include <string>

class MuteConverter : public IConverter {
private:
    int startSample;
    int endSample;

public:
    MuteConverter() : startSample(0), endSample(0) {}

    void process(std::vector<int16_t>& buffer, size_t positionInSamples) override;
    void setParameters(const std::vector<std::string>& params) override;
    void setAdditionalData(const std::vector<int16_t>& data) override {}

    static IConverter* create() {
        return new MuteConverter();
    }

    static std::string getDescription() {
        return "Mutes audio in the specified time interval (sets samples to zero)";
    }

    static std::string getUsage() {
        return "mute <start_seconds> <end_seconds>";
    }

    static std::string getName() {
        return "mute";
    }
};