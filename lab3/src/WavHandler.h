#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

class WavHandler {
private:
    struct WavHeader {
        char chunkId[4];
        uint32_t chunkSize;
        char format[4];

        char subchunk1Id[4];
        uint32_t subchunk1Size;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;

        char subchunk2Id[4];
        uint32_t subchunk2Size;
    };

    std::string filename;
    WavHeader header;
    std::ifstream inputFile;
    std::ofstream outputFile;
    bool isWriting;
    uint32_t samplesWritten;

    void validateHeader() const;
    void writeHeader();
    void updateHeader();

public:
    WavHandler(const std::string& filename);

    WavHandler(const std::string& filename, const WavHandler& templateWav);

    ~WavHandler();

    bool readSamples(std::vector<int16_t>& buffer, size_t count);
    void writeSamples(const std::vector<int16_t>& buffer);

    uint32_t getTotalSamples() const;
    uint32_t getSampleRate() const { return header.sampleRate; }
    bool isSupportedFormat() const;
};