#include "MuteConverter.h"
#include "ConverterFactory.h"
#include "ConverterRegistry.h"
#include <stdexcept>

namespace {
    IConverter* createMuteConverter() {
        return new MuteConverter();
    }
    bool registeredFactory = ConverterFactory::getInstance()->registerCreator(
        "mute", createMuteConverter);

    bool registeredInfo = []() -> bool {
        ConverterRegistry::registerConverter(
            "mute",
            "Mutes audio in the specified time interval (sets samples to zero)",
            "mute <start_seconds> <end_seconds>");
        return true;
    }();
}

void MuteConverter::setParameters(const std::vector<std::string>& params) {
    if (params.size() != 2) {
        throw std::invalid_argument("Mute converter requires 2 parameters");
    }

    float startSec = std::stof(params[0]);
    float endSec = std::stof(params[1]);

    if (startSec < 0 || endSec < 0 || startSec > endSec) {
        throw std::invalid_argument("Invalid time interval");
    }

    startSample = static_cast<int>(startSec * 44100);
    endSample = static_cast<int>(endSec * 44100);
}

void MuteConverter::process(std::vector<int16_t>& buffer, size_t positionInSamples) {
    size_t bufferEnd = positionInSamples + buffer.size();

    if (bufferEnd <= startSample || positionInSamples >= endSample) {
        return;
    }

    size_t bufferStartIdx = 0;
    if (startSample > positionInSamples) {
        bufferStartIdx = startSample - positionInSamples;
    }

    size_t bufferEndIdx = buffer.size();
    if (endSample < bufferEnd) {
        bufferEndIdx = endSample - positionInSamples;
    }

    for (size_t i = bufferStartIdx; i < bufferEndIdx; ++i) {
        buffer[i] = 0;
    }
}