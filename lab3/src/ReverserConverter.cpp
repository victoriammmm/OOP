#include "ReverserConverter.h"
#include "ConverterFactory.h"
#include "ConverterRegistry.h"
#include <algorithm>
#include <stdexcept>

namespace {
    IConverter* createReverseConverter() {
        return new ReverserConverter();
    }

    bool registeredFactory = ConverterFactory::getInstance()->registerCreator(
        "reverse", createReverseConverter);

    bool registeredInfo = []() -> bool {
        ConverterRegistry::registerConverter(
            "reverse",
            "Reverses audio in the specified time interval",
            "reverse <start_seconds> <end_seconds>");
        return true;
    }();
}

void ReverserConverter::setParameters(const std::vector<std::string>& params) {
    if (params.size() != 2) {
        throw std::invalid_argument("Reverse converter requires exactly 2 parameters: start end");
    }

    try {
        float start = std::stof(params[0]);
        float end = std::stof(params[1]);

        if (start < 0 || end < 0) {
            throw std::invalid_argument("Time values cannot be negative");
        }
        if (start > end) {
            throw std::invalid_argument("Start time must be less than end time");
        }

        startSample = static_cast<int>(start * 44100);
        endSample = static_cast<int>(end * 44100);

    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Invalid number format for time parameters");
    } catch (const std::out_of_range&) {
        throw std::invalid_argument("Time value out of range");
    }
}

void ReverserConverter::process(std::vector<int16_t>& buffer, size_t positionInSamples) {
    size_t bufferEnd = positionInSamples + buffer.size();

    if (bufferEnd <= startSample || positionInSamples >= endSample) {
        return;
    }

    size_t startIdx = (startSample > positionInSamples)
        ? startSample - positionInSamples : 0;
    size_t endIdx = (endSample < bufferEnd)
        ? endSample - positionInSamples : buffer.size();

    if (startIdx >= endIdx) {
        return;
    }

    std::reverse(buffer.begin() + startIdx, buffer.begin() + endIdx);
}