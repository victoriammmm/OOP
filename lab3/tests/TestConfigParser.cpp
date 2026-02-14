#include <gtest/gtest.h>
#include "../ConfigParser.h"

TEST(TestConfigParser, ParseValidConfig) {
    std::ofstream config("config.txt");
    config << "mute 0 10\n";
    config << "mix $2 5\n";
    config << "reverse 2 8";
    config.close();
    
    ConfigParser parser("config.txt");
    auto commands = parser.parse();
    
    EXPECT_EQ(commands.size(), 3);
    EXPECT_EQ(commands[0].name, "mute");
    EXPECT_EQ(commands[1].name, "mix");
    EXPECT_EQ(commands[2].name, "reverse");
    
    remove("config.txt");
}

TEST(TestConfigParser, ParseIgnoresComments) {
    std::ofstream config("config.txt");
    config << "# comment\n";
    config << "mute 0 10\n";
    config << "  # indented comment\n";
    config << "mix $2 5";
    config.close();
    
    ConfigParser parser("config.txt");
    auto commands = parser.parse();
    
    EXPECT_EQ(commands.size(), 2);
    
    remove("config.txt");
}

TEST(TestConfigParser, ParseHandlesSpaces) {
    std::ofstream config("config.txt");
    config << "  mute   0   10  \n";
    config << "\tmix\t$2\t5\t";
    config.close();
    
    ConfigParser parser("config.txt");
    auto commands = parser.parse();
    
    EXPECT_EQ(commands.size(), 2);
    
    remove("config.txt");
}

TEST(TestConfigParser, ParseEmptyFileThrows) {
    std::ofstream config("config.txt");
    config.close();
    
    ConfigParser parser("config.txt");
    EXPECT_THROW(parser.parse(), ConfigException);
    
    remove("config.txt");
}

TEST(TestConfigParser, ParseInvalidCommandThrows) {
    std::ofstream config("config.txt");
    config << "unknown 0 10";
    config.close();
    
    ConfigParser parser("config.txt");
    EXPECT_THROW(parser.parse(), ConfigException);
    
    remove("config.txt");
}

TEST(TestConfigParser, ParseMissingFileThrows) {
    ConfigParser parser("nonexistent.txt");
    EXPECT_THROW(parser.parse(), ConfigException);
}