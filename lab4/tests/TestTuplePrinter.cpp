#include <gtest/gtest.h>
#include <sstream>
#include <tuple>
#include "TuplePrinter.h"

class TupleOutputTest : public ::testing::Test {
protected:
    std::stringstream buffer;
};

TEST_F(TupleOutputTest, HeterogeneousTuplePrint) {
    std::tuple<int, double, std::string> t{7, 2.718, "world"};
    buffer << t;
    EXPECT_EQ(buffer.str(), "(7, 2.718, world)");
}

TEST_F(TupleOutputTest, OneElementTuple) {
    std::tuple<std::string> t{"solo"};
    buffer << t;
    EXPECT_EQ(buffer.str(), "(solo)");
}

TEST_F(TupleOutputTest, NoElementsTuple) {
    std::tuple<> t;
    buffer << t;
    EXPECT_EQ(buffer.str(), "()");
}

TEST_F(TupleOutputTest, FiveIntegersTuple) {
    std::tuple<int, int, int, int, int> t{9, 8, 7, 6, 5};
    buffer << t;
    EXPECT_EQ(buffer.str(), "(9, 8, 7, 6, 5)");
}

TEST_F(TupleOutputTest, BoolCharAndNumber) {
    std::tuple<bool, char, int> t{false, 'Z', 256};
    buffer << t;
    EXPECT_EQ(buffer.str(), "(0, Z, 256)");
}