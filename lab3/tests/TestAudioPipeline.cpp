#include <gtest/gtest.h>
#include "../AudioPipeline.h"
#include <fstream>

class TestAudioPipeline : public ::testing::Test {
protected:
    void SetUp() override {
        createTestWav("test1.wav");
        createTestWav("test2.wav");
    }
    
    void TearDown() override {
        remove("test1.wav");
        remove("test2.wav");
        remove("output.wav");
        remove("config.txt");
    }
    
    void createTestWav(const std::string& name) {
        std::ofstream file(name, std::ios::binary);
        char header[44] = {
            'R','I','F','F', 0x24,0,0,0, 'W','A','V','E',
            'f','m','t',' ', 0x10,0,0,0, 0x01,0, 0x01,0,
            0x44,0xAC,0,0, 0x88,0x58,0x01,0, 0x02,0, 0x10,0,
            'd','a','t','a', 0x00,0,0,0
        };
        file.write(header, 44);
        file.close();
    }
};

TEST_F(TestAudioPipeline, ProcessWithMute) {
    std::ofstream config("config.txt");
    config << "mute 0 1";
    config.close();
    
    AudioPipeline pipeline("config.txt", {"test1.wav", "test2.wav"}, "output.wav");
    EXPECT_NO_THROW(pipeline.build());
    EXPECT_NO_THROW(pipeline.process());
    
    std::ifstream result("output.wav");
    EXPECT_TRUE(result.is_open());
}

TEST_F(TestAudioPipeline, ProcessWithMix) {
    std::ofstream config("config.txt");
    config << "mix $2 0";
    config.close();
    
    AudioPipeline pipeline("config.txt", {"test1.wav", "test2.wav"}, "output.wav");
    EXPECT_NO_THROW(pipeline.build());
    EXPECT_NO_THROW(pipeline.process());
    
    std::ifstream result("output.wav");
    EXPECT_TRUE(result.is_open());
}

TEST_F(TestAudioPipeline, ProcessWithReverse) {
    std::ofstream config("config.txt");
    config << "reverse 0 1";
    config.close();
    
    AudioPipeline pipeline("config.txt", {"test1.wav", "test2.wav"}, "output.wav");
    EXPECT_NO_THROW(pipeline.build());
    EXPECT_NO_THROW(pipeline.process());
    
    std::ifstream result("output.wav");
    EXPECT_TRUE(result.is_open());
}

TEST_F(TestAudioPipeline, FullPipeline) {
    std::ofstream config("config.txt");
    config << "mute 0 1\n";
    config << "mix $2 1\n";
    config << "reverse 2 3";
    config.close();
    
    AudioPipeline pipeline("config.txt", {"test1.wav", "test2.wav"}, "output.wav");
    EXPECT_NO_THROW(pipeline.build());
    EXPECT_NO_THROW(pipeline.process());
    
    std::ifstream result("output.wav");
    EXPECT_TRUE(result.is_open());
}

TEST_F(TestAudioPipeline, InvalidConfigThrows) {
    std::ofstream config("config.txt");
    config << "unknown 0 1";
    config.close();
    
    AudioPipeline pipeline("config.txt", {"test1.wav"}, "output.wav");
    EXPECT_THROW(pipeline.build(), ConfigException);
}