#include <gtest/gtest.h>
#include "../MuteConverter.h"
#include "../MixConverter.h"
#include "../ReverserConverter.h"

TEST(MuteConverterTest, SetValidParameters) {
    MuteConverter converter;
    EXPECT_NO_THROW(converter.setParameters({"0", "10"}));
}

TEST(MuteConverterTest, SetInvalidParametersThrows) {
    MuteConverter converter;
    EXPECT_THROW(converter.setParameters({"10", "0"}), std::invalid_argument);
    EXPECT_THROW(converter.setParameters({"0"}), std::invalid_argument);
}

TEST(MuteConverterTest, MuteProcess) {
    MuteConverter converter;
    converter.setParameters({"0", "5"});
    
    std::vector<int16_t> buffer(100, 1000);
    converter.process(buffer, 0);
    
    for (int i = 0; i < 5 * 44100 / 100; ++i) {
        EXPECT_EQ(buffer[i], 0);
    }
}

TEST(MixConverterTest, SetValidParameters) {
    MixConverter converter;
    EXPECT_NO_THROW(converter.setParameters({"$2", "0"}));
}

TEST(MixConverterTest, SetInvalidParametersThrows) {
    MixConverter converter;
    EXPECT_THROW(converter.setParameters({"2", "0"}), std::invalid_argument);
    EXPECT_THROW(converter.setParameters({"$abc", "0"}), std::invalid_argument);
}

TEST(MixConverterTest, MixProcessWithoutData) {
    MixConverter converter;
    converter.setParameters({"$1", "0"});
    
    std::vector<int16_t> buffer = {1000, 2000, 3000};
    std::vector<int16_t> original = buffer;
    
    converter.process(buffer, 0);
    EXPECT_EQ(buffer, original);
}

TEST(ReverserConverterTest, SetValidParameters) {
    ReverserConverter converter;
    EXPECT_NO_THROW(converter.setParameters({"0", "10"}));
}

TEST(ReverserConverterTest, SetInvalidParametersThrows) {
    ReverserConverter converter;
    EXPECT_THROW(converter.setParameters({"10", "0"}), std::invalid_argument);
    EXPECT_THROW(converter.setParameters({"0"}), std::invalid_argument);
}

TEST(ReverserConverterTest, ReverseProcess) {
    ReverserConverter converter;
    converter.setParameters({"0", "3"});
    
    std::vector<int16_t> buffer = {1, 2, 3, 4, 5, 6};
    std::vector<int16_t> expected = {3, 2, 1, 4, 5, 6};
    
    converter.process(buffer, 0);
    
    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(buffer[i], expected[i]);
    }
}