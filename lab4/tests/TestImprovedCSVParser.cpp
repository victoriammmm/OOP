#include <gtest/gtest.h>
#include <sstream>
#include "CSVParser.h"

TEST(ImprovedCsvReaderTest, SemicolonDelimiter) {
    std::stringstream ss;
    ss << "3;Mark;9.9";

    CsvFormat fmt;
    fmt.columnSep = ';';

    CsvReader<int, std::string, double> reader(ss, fmt);

    auto row = *reader.begin();
    EXPECT_EQ(std::get<1>(row), "Mark");
}

TEST(ImprovedCsvReaderTest, PipeSeparatedRows) {
    std::stringstream ss;
    ss << "1,A,10|2,B,20|3,C,30";

    CsvFormat fmt(',', '|');

    CsvReader<int, std::string, int> reader(ss, fmt);

    std::vector<int> values;
    for (auto& r : reader)
        values.push_back(std::get<2>(r));

    EXPECT_EQ(values.size(), 3);
    EXPECT_EQ(values[2], 30);
}

TEST(ImprovedCsvReaderTest, CommaInsideQuotes) {
    std::stringstream ss;
    ss << "7,\"Last, First\",100";

    CsvReader<int, std::string, int> reader(ss);

    auto row = *reader.begin();
    EXPECT_EQ(std::get<1>(row), "Last, First");
}

TEST(ImprovedCsvReaderTest, EscapedQuotesResolved) {
    std::stringstream ss;
    ss << "1,\"He said \"\"Hello\"\"\",5";

    CsvReader<int, std::string, int> reader(ss);

    auto row = *reader.begin();
    EXPECT_EQ(std::get<1>(row), "He said \"Hello\"");
}

TEST(ImprovedCsvReaderTest, UnclosedQuotesFail) {
    std::stringstream ss;
    ss << "4,\"Broken field,7";

    CsvReader<int, std::string> reader(ss);

    EXPECT_THROW({
        auto it = reader.begin();
        *it;
    }, CsvReadException);
}

TEST(ImprovedCsvReaderTest, HeaderSkippedCorrectly) {
    std::stringstream ss;
    ss << "X,Y,Z\n"
       << "8,Kate,11\n"
       << "9,Liam,13";

    CsvReader<int, std::string, int> reader(ss, 1);

    auto row = *reader.begin();
    EXPECT_EQ(std::get<1>(row), "Kate");
}

TEST(ImprovedCsvReaderTest, ErrorNotOnlyFirstRow) {
    std::stringstream ss;
    ss << "1,Valid,5\n"
       << "2,Invalid";

    CsvReader<int, std::string, int> reader(ss);

    EXPECT_THROW({
        auto it = reader.begin();
        ++it;
    }, CsvReadException);
}