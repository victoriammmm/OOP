#include <gtest/gtest.h>
#include "LoadFromFile.h"
#include "Universe.h"
#include <fstream>

class LoaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        testFile = "test_input.txt";
        std::ofstream file(testFile);
        file << "Life 1.06\n";
        file << "#N Test Universe\n";
        file << "#R B3/S23\n";
        file << "#F 5/5\n";
        file << "1 1\n";
        file << "2 2\n";
        file << "3 3\n";
        file.close();
    }
    
    void TearDown() override {
        std::remove(testFile.c_str());
    }
    
    std::string testFile;
};

TEST_F(LoaderTest, LoadValidFile) {
    Universe universe(1, 1, "B3/S23");
    EXPECT_NO_THROW(UniverseLoader::loadFromFile(universe, testFile));
    EXPECT_EQ(universe.getName(), "Test Universe");
    EXPECT_EQ(universe.getRule(), "B3/S23");
    EXPECT_EQ(universe.getWidth(), 5);
    EXPECT_EQ(universe.getHeight(), 5);
    EXPECT_EQ(universe.getField().getCell(1, 1), CellState::Alive);
    EXPECT_EQ(universe.getField().getCell(2, 2), CellState::Alive);
    EXPECT_EQ(universe.getField().getCell(3, 3), CellState::Alive);
    EXPECT_EQ(universe.getField().getCell(0, 0), CellState::Dead);
}

TEST_F(LoaderTest, LoadNonexistentFile) {
    Universe universe(1, 1, "B3/S23");
    EXPECT_THROW(UniverseLoader::loadFromFile(universe, "nonexistent.txt"), std::runtime_error);
}

TEST_F(LoaderTest, LoadInvalidFormat) {
    std::string badFile = "bad_format.txt";
    std::ofstream file(badFile);
    file << "Invalid Format\n";
    file.close();
    
    Universe universe(1, 1, "B3/S23");
    EXPECT_THROW(UniverseLoader::loadFromFile(universe, badFile), std::runtime_error);
    
    std::remove(badFile.c_str());
}

TEST_F(LoaderTest, LoadWithoutSize) {
    std::string noSizeFile = "no_size.txt";
    std::ofstream file(noSizeFile);
    file << "Life 1.06\n";
    file << "#N Test\n";
    file << "#R B3/S23\n";
    file << "1 1\n";
    file.close();
    
    Universe universe(1, 1, "B3/S23");
    EXPECT_NO_THROW(UniverseLoader::loadFromFile(universe, noSizeFile));
    EXPECT_EQ(universe.getWidth(), 20);
    EXPECT_EQ(universe.getHeight(), 20);
    EXPECT_EQ(universe.getField().getCell(1, 1), CellState::Alive);
    
    std::remove(noSizeFile.c_str());
}