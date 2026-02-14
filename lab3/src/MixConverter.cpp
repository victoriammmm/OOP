#include "MixConverter.h"
#include "ConverterFactory.h"
#include "ConverterRegistry.h"
#include <stdexcept>
#include <algorithm>

namespace {
    IConverter* createMixConverter() {
        return new MixConverter();
    }
    bool registeredFactory = ConverterFactory::getInstance()->registerCreator(
        "mix", createMixConverter);

    bool registeredInfo = []() -> bool {
        ConverterRegistry::registerConverter(
            "mix",
            "Mixes main stream with additional stream starting from specified time",
            "mix $<file_number> <start_seconds>");
        return true;
    }();
}

void MixConverter::setParameters(const std::vector<std::string>& params) {
    if (params.size() != 2) {
        throw std::invalid_argument("Mix converter requires exactly 2 parameters: $file start");
    }

    std::string fileRef = params[0];
    if (fileRef.empty() || fileRef[0] != '$') {
        throw std::invalid_argument("First parameter must be file reference ($1, $2, etc.)");
    }

    try {
        float start = std::stof(params[1]);
        if (start < 0) {
            throw std::invalid_argument("Start time cannot be negative");
        }

        startSample = static_cast<int>(start * 44100);

    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Invalid number format for start time");
    } catch (const std::out_of_range&) {
        throw std::invalid_argument("Start time value out of range");
    }
}

void MixConverter::process(std::vector<int16_t>& buffer, size_t positionInSamples) {
    if (additionalStream.empty()) {
        return;
    }

    if (positionInSamples + buffer.size() <= startSample) {
        return;
    }

    size_t bufferStartIdx = 0;
    if (startSample > positionInSamples) {
        bufferStartIdx = startSample - positionInSamples;
    }

    size_t streamStartIdx = positionInSamples + bufferStartIdx - startSample;

    for (size_t i = bufferStartIdx; i < buffer.size(); ++i) {
        if (streamStartIdx >= additionalStream.size()) {
            break;
        }

        int32_t mixed = (static_cast<int32_t>(buffer[i]) + static_cast<int32_t>(additionalStream[streamStartIdx])/2);

        buffer[i] = static_cast<int16_t>(mixed);
        streamStartIdx++;
    }
}