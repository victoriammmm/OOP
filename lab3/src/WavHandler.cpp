#include "WavHandler.h"
#include "Exceptions.h"
#include <cstring>
#include <iostream>

WavHandler::WavHandler(const std::string& filename)
    : filename(filename), isWriting(false), samplesWritten(0) {

    inputFile.open(filename, std::ios::binary);
    if (!inputFile.is_open()) {
        throw WavException("Cannot open file: " + filename);
    }

    inputFile.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

    if (!inputFile) {
        throw WavException("Failed to read WAV header from: " + filename);
    }

    validateHeader();

    if (!isSupportedFormat()) {
        throw WavException("Unsupported WAV format. Required: PCM mono 16-bit 44100Hz");
    }

    inputFile.seekg(sizeof(WavHeader));

    std::cout << "Opened WAV file: " << filename << "\n";
}

WavHandler::WavHandler(const std::string& filename, const WavHandler& templateWav)
    : filename(filename), isWriting(true), samplesWritten(0) {

    header = templateWav.header;

    header.subchunk2Size = 0;
    header.chunkSize = 36;

    outputFile.open(filename, std::ios::binary);
    if (!outputFile.is_open()) {
        throw WavException("Cannot create output file: " + filename);
    }

    writeHeader();

    std::cout << "Created output WAV file: " << filename << "\n";
}

WavHandler::~WavHandler() {
    if (outputFile.is_open()) {
        updateHeader();
        outputFile.close();
    }
    if (inputFile.is_open()) {
        inputFile.close();
    }
}

void WavHandler::validateHeader() const {
    if (std::strncmp(header.chunkId, "RIFF", 4) != 0) {
        throw WavException("Invalid RIFF header in file");
    }

    if (std::strncmp(header.format, "WAVE", 4) != 0) {
        throw WavException("Not a WAVE file");
    }

    if (header.audioFormat != 1) {
        throw WavException("Not a PCM file (audio format must be 1)");
    }

    if (header.subchunk2Size == 0) {
        throw WavException("WAV file has no audio data");
    }
}

bool WavHandler::isSupportedFormat() const {
    return (header.numChannels == 1 && header.bitsPerSample == 16 && header.sampleRate == 44100);
}

void WavHandler::writeHeader() {
    if (!outputFile.is_open()) return;

    outputFile.seekp(0);
    outputFile.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));
}

void WavHandler::updateHeader() {
    if (!outputFile.is_open()) return;

    header.subchunk2Size = samplesWritten * sizeof(int16_t);
    header.chunkSize = header.subchunk2Size + 36;

    outputFile.seekp(0);
    outputFile.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));
    outputFile.seekp(0, std::ios::end);
}

bool WavHandler::readSamples(std::vector<int16_t>& buffer, size_t count) {
    buffer.resize(count);

    if (!inputFile.is_open()) {
        return false;
    }

    inputFile.read(reinterpret_cast<char*>(buffer.data()),
                   count * sizeof(int16_t));

    size_t bytesRead = inputFile.gcount();
    size_t samplesRead = bytesRead / sizeof(int16_t);

    if (samplesRead < count) {
        buffer.resize(samplesRead);
        return false;
    }

    return true;
}

void WavHandler::writeSamples(const std::vector<int16_t>& buffer) {
    if (!outputFile.is_open()) {
        throw WavException("Output file not open for writing");
    }

    if (buffer.empty()) {
        return;
    }

    outputFile.write(reinterpret_cast<const char*>(buffer.data()),
                     buffer.size() * sizeof(int16_t));

    if (!outputFile.good()) {
        throw WavException("Failed to write samples to file");
    }
    samplesWritten += buffer.size();
}

uint32_t WavHandler::getTotalSamples() const {
    if (isWriting) {
        return samplesWritten;
    }

    if (header.bitsPerSample == 0) {
        return 0;
    }

    uint32_t bytesPerSample = header.bitsPerSample / 8;
    if (bytesPerSample == 0) {
        return 0;
    }

    return header.subchunk2Size / bytesPerSample;
}