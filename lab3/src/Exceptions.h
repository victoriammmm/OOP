#pragma once

#include <stdexcept>
#include <string>

class SoundProcessorException : public std::runtime_error {
public:
    explicit SoundProcessorException(const std::string& msg) 
        : std::runtime_error(msg) {}
};

class ConfigException : public SoundProcessorException {
public:
    explicit ConfigException(const std::string& msg) 
        : SoundProcessorException("\nConfig error, " + msg) {}
};

class WavException : public SoundProcessorException {
public:
    explicit WavException(const std::string& msg)
        : SoundProcessorException("\nWAV format error, " + msg) {}
};

class ConverterException : public SoundProcessorException {
public:
    explicit ConverterException(const std::string& msg)
        : SoundProcessorException("\nConverter error, " + msg) {}
};