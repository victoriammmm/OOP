#include <gtest/gtest.h>
#include "../ConverterFactory.h"

TEST(TestFactory, CreateMuteConverter) {
    auto factory = ConverterFactory::getInstance();
    auto converter = factory->create("mute");
    EXPECT_NE(converter, nullptr);
}

TEST(TestFactory, CreateMixConverter) {
    auto factory = ConverterFactory::getInstance();
    auto converter = factory->create("mix");
    EXPECT_NE(converter, nullptr);
}

TEST(TestFactory, CreateReverseConverter) {
    auto factory = ConverterFactory::getInstance();
    auto converter = factory->create("reverse");
    EXPECT_NE(converter, nullptr);
}

TEST(TestFactory, CreateUnknownConverterReturnsNull) {
    auto factory = ConverterFactory::getInstance();
    auto converter = factory->create("unknown");
    EXPECT_EQ(converter, nullptr);
}

TEST(TestFactory, GetAvailableConverters) {
    auto factory = ConverterFactory::getInstance();
    auto converters = factory->getAvailableIds();
    
    EXPECT_FALSE(converters.empty());
    EXPECT_NE(std::find(converters.begin(), converters.end(), "mute"), converters.end());
    EXPECT_NE(std::find(converters.begin(), converters.end(), "mix"), converters.end());
    EXPECT_NE(std::find(converters.begin(), converters.end(), "reverse"), converters.end());
}

TEST(TestFactory, HasConverter) {
    auto factory = ConverterFactory::getInstance();
    EXPECT_TRUE(factory->hasCreator("mute"));
    EXPECT_TRUE(factory->hasCreator("mix"));
    EXPECT_TRUE(factory->hasCreator("reverse"));
    EXPECT_FALSE(factory->hasCreator("unknown"));
}