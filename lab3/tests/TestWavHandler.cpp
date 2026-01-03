#include <gtest/gtest.h>
#include "../WavHandler.h"

TEST(TestWavHandler, OpenValidFile) {
    std::ofstream file("test.wav", std::ios::binary);
    char header[44] = {
        'R','I','F','F', 0x24,0,0,0, 'W','A','V','E',
        'f','m','t',' ', 0x10,0,0,0, 0x01,0, 0x01,0,
        0x44,0xAC,0,0, 0x88,0x58,0x01,0, 0x02,0, 0x10,0,
        'd','a','t','a', 0x00,0,0,0
    };
    file.write(header, 44);
    file.close();
    
    EXPECT_NO_THROW(WavHandler wav("test.wav"));
    
    remove("test.wav");
}

TEST(TestWavHandler, OpenMissingFileThrows) {
    EXPECT_THROW(WavHandler wav("missing.wav"), WavException);
}

TEST(TestWavHandler, OpenInvalidWavThrows) {
    std::ofstream file("bad.wav");
    file << "not a wav file";
    file.close();
    
    EXPECT_THROW(WavHandler wav("bad.wav"), WavException);
    
    remove("bad.wav");
}

TEST(TestWavHandler, ReadSamples) {
    std::ofstream file("test.wav", std::ios::binary);
    char header[44] = {
        'R','I','F','F', 0x2C,0,0,0, 'W','A','V','E',
        'f','m','t',' ', 0x10,0,0,0, 0x01,0, 0x01,0,
        0x44,0xAC,0,0, 0x88,0x58,0x01,0, 0x02,0, 0x10,0,
        'd','a','t','a', 0x08,0,0,0
    };
    file.write(header, 44);
    int16_t samples[4] = {1000, 2000, 3000, 4000};
    file.write((char*)samples, 8);
    file.close();
    
    WavHandler wav("test.wav");
    std::vector<int16_t> buffer(4);
    
    EXPECT_TRUE(wav.readSamples(buffer, 4));
    
    remove("test.wav");
}

TEST(TestWavHandler, CreateOutputFile) {
    std::ofstream in("input.wav", std::ios::binary);
    char header[44] = {
        'R','I','F','F', 0x24,0,0,0, 'W','A','V','E',
        'f','m','t',' ', 0x10,0,0,0, 0x01,0, 0x01,0,
        0x44,0xAC,0,0, 0x88,0x58,0x01,0, 0x02,0, 0x10,0,
        'd','a','t','a', 0x00,0,0,0
    };
    in.write(header, 44);
    in.close();
    
    WavHandler input("input.wav");
    WavHandler output("output.wav", input);
    
    std::vector<int16_t> samples = {1000, 2000, 3000};
    output.writeSamples(samples);
    
    std::ifstream check("output.wav");
    EXPECT_TRUE(check.is_open());
    check.close();
    
    remove("input.wav");
    remove("output.wav");
}