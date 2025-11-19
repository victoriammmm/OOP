#include "filereader.h"
#include "writetocsv.h"
#include "wordcounter.h"
#include <gtest/gtest.h>
#include <fstream>

class FileReaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream file("test_file.txt");
        file << "Hello world!\nTest line.";
        file.close();
    }
    void TearDown() override {
        std::remove("test_file.txt");
    }
};

TEST_F(FileReaderTest, ReadExistingFile) {
    FileReader reader;
    EXPECT_TRUE(reader.readFile("test_file.txt"));
    auto lines = reader.getLines();
    EXPECT_EQ(lines.size(), 2);
    EXPECT_EQ(lines.front(), "Hello world!");
}

TEST_F(FileReaderTest, ReadNonExistingFile) {
    FileReader reader;
    EXPECT_FALSE(reader.readFile("non_existing_file_12345.txt"));
}

class WordCounterTest : public ::testing::Test {
protected:
    WordCounter counter;
};

TEST_F(WordCounterTest, CountBasicWords) {
    std::list<std::string> lines = {"hello world", "hello test"};
    counter.countWords(lines);
    EXPECT_EQ(counter.getTotalWords(), 4);
}

TEST_F(WordCounterTest, CountWordsWithDelimiters) {
    std::list<std::string> lines = {"hello, world! test123;"};
    counter.countWords(lines);
    EXPECT_EQ(counter.getTotalWords(), 3);
}

TEST_F(WordCounterTest, CaseInsensitive) {
    std::list<std::string> lines = {"Hello HELLO hello"};
    counter.countWords(lines);
    counter.calculateFrequency();
    counter.sortByFrequency();
    auto words = counter.getSortedWords();
    ASSERT_FALSE(words.empty());
    EXPECT_EQ(words[0].first, "hello");
    EXPECT_EQ(words[0].second.first, 3);
}

TEST_F(WordCounterTest, CalculateFrequency) {
    std::list<std::string> lines = {"a a b"};
    counter.countWords(lines);
    counter.calculateFrequency();
    auto words = counter.getSortedWords();
    EXPECT_NEAR(words[0].second.second, 66.666, 0.1);
}

TEST_F(WordCounterTest, SortByFrequency) {
    std::list<std::string> lines = {"c b a c b c"};
    counter.countWords(lines);
    counter.calculateFrequency();
    counter.sortByFrequency();
    auto words = counter.getSortedWords();
    ASSERT_EQ(words.size(), 3);
    EXPECT_EQ(words[0].first, "c");
    EXPECT_EQ(words[0].second.first, 3);
    EXPECT_EQ(words[1].first, "b");
    EXPECT_EQ(words[1].second.first, 2);
    EXPECT_EQ(words[2].first, "a");
    EXPECT_EQ(words[2].second.first, 1);
}

TEST_F(WordCounterTest, EmptyLines) {
    std::list<std::string> lines = {"", "   ", "valid word", ""};
    counter.countWords(lines);
    EXPECT_EQ(counter.getTotalWords(), 2);
}

class CSVWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::list<std::string> lines = {
            "hello world python java",
            "hello test cpp rust",
            "world programming coding",
            "test development debug",
            "hello open source"
        };
        counter.countWords(lines);
        counter.calculateFrequency();
        counter.sortByFrequency();
    }
    void TearDown() override {
        std::remove("test_output.csv");
    }
    WordCounter counter;
};

TEST_F(CSVWriterTest, WriteCSVFile) {
    EXPECT_TRUE(CSVWriter::writeCSV("test_output.csv", counter));
}

TEST_F(CSVWriterTest, CSVFileContent) {
    CSVWriter::writeCSV("test_output.csv", counter);
    std::ifstream file("test_output.csv");
    ASSERT_TRUE(file.good());
    std::string header;
    std::getline(file, header);
    EXPECT_EQ(header, "Word,Frequency,Frequency(%)");
}

TEST_F(CSVWriterTest, WriteToInvalidPath) {
    EXPECT_FALSE(CSVWriter::writeCSV("/invalid/path/test.csv", counter));
    EXPECT_FALSE(CSVWriter::writeCSV("C:/Windows/system32/test.csv", counter));
    EXPECT_FALSE(CSVWriter::writeCSV("non_existing_folder/test.csv", counter));
}

TEST(IntegrationTest, FullPipeline) {
    std::ofstream input("integration_test.txt");
    input << "First sentence.\nSecond sentence.";
    input.close();
    FileReader reader;
    ASSERT_TRUE(reader.readFile("integration_test.txt"));
    WordCounter counter;
    counter.countWords(reader.getLines());
    counter.calculateFrequency();
    counter.sortByFrequency();
    EXPECT_TRUE(CSVWriter::writeCSV("integration_output.csv", counter));
    EXPECT_GT(counter.getTotalWords(), 0);
    std::remove("integration_test.txt");
    std::remove("integration_output.csv");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

