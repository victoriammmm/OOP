#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include "CSVParser.h"

TEST(CsvReaderTest, SimpleTableRead) {
    std::stringstream ss;
    ss << "10,Alex,88.5\n"
       << "20,Bella,91.2\n"
       << "30,Chris,76.0";

    CsvReader<int, std::string, double> reader(ss);

    std::vector<std::tuple<int, std::string, double>> rows;
    for (const auto& r : reader)
        rows.push_back(r);

    ASSERT_EQ(rows.size(), 3);
    EXPECT_EQ(std::get<0>(rows[0]), 10);
    EXPECT_EQ(std::get<1>(rows[0]), "Alex");
    EXPECT_DOUBLE_EQ(std::get<2>(rows[0]), 88.5);
}

TEST(CsvReaderTest, SkipHeaderLine) {
    std::stringstream ss;
    ss << "ID,Name,Score\n"
       << "1,Dylan,42\n"
       << "2,Erica,55";

    CsvReader<int, std::string, int> reader(ss, 1);

    auto it = reader.begin();
    EXPECT_EQ(std::get<0>(*it), 1);
    EXPECT_EQ(std::get<1>(*it), "Dylan");
}

TEST(CsvReaderTest, MixedTypeParsing) {
    std::stringstream ss;
    ss << "5,3.5,Alpha\n"
       << "6,4.1,Beta";

    CsvReader<int, double, std::string> reader(ss);

    auto it = reader.begin();
    EXPECT_EQ(std::get<1>(*it), 3.5);
}

TEST(CsvReaderTest, EmptyStream) {
    std::stringstream ss;
    CsvReader<int, std::string> reader(ss);

    int counter = 0;
    for (auto&& row : reader)
        counter++;

    EXPECT_EQ(counter, 0);
}

TEST(CsvReaderTest, SingleLineOnly) {
    std::stringstream ss;
    ss << "99,unique";

    CsvReader<int, std::string> reader(ss);

    auto it = reader.begin();
    EXPECT_EQ(std::get<0>(*it), 99);
    EXPECT_EQ(std::get<1>(*it), "unique");
}

TEST(CsvReaderTest, WrongColumnCountThrows) {
    std::stringstream ss;
    ss << "1,TooFew\n";

    CsvReader<int, std::string, int> reader(ss);

    EXPECT_THROW({for (auto&& r : reader) {}}, CsvReadException);
}
