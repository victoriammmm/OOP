#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

class IConverter {
public:
    virtual ~IConverter() = default;

    virtual void process(std::vector<int16_t>& buffer, size_t positionInSamples) = 0;
    virtual void setParameters(const std::vector<std::string>& params) = 0;
    virtual void setAdditionalData(const std::vector<int16_t>& data) {}
};