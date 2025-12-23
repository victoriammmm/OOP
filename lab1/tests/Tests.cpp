#include "gtest/gtest.h"
#include "BitArray.h"
#include <stdexcept>

class BitArrayTest : public testing::Test {
protected:
    BitArray* bitArray = nullptr;
    BitArray* toCompareBitArray = nullptr;

    void SetUp() override {
        bitArray = new BitArray(4, 0);
        toCompareBitArray = new BitArray(4, 1);
    }

    void TearDown() override {
        delete bitArray;
        delete toCompareBitArray;
    }
};
// проверка преобразования в строку
TEST_F(BitArrayTest, toString) {
    EXPECT_EQ("0000", bitArray->to_string());
}
// создает копию
TEST_F(BitArrayTest, copyConstructor) {
    BitArray newBitArray = BitArray(*bitArray);
    EXPECT_EQ(bitArray->to_string(), newBitArray.to_string());
}
// конструктор
TEST_F(BitArrayTest, defaultConstructor) {
    BitArray newBitArray = BitArray();
    EXPECT_EQ("", newBitArray.to_string());
    EXPECT_EQ(0, newBitArray.size());
}

TEST_F(BitArrayTest, operatorGet) {
    EXPECT_EQ(false, (*bitArray)[0]);
}

TEST_F(BitArrayTest, operatorEqualEqualFalse) {
    EXPECT_EQ(false, (*bitArray) == (*toCompareBitArray));
}

TEST_F(BitArrayTest, operatorEqualEqualFalseDifferentSize) {
    toCompareBitArray->resize(6, false);
    EXPECT_EQ(false, (*bitArray) == (*toCompareBitArray));
}

TEST_F(BitArrayTest, operatorEqualEqualTrue) {
    EXPECT_EQ(true, (*bitArray) == (*bitArray));
}

TEST_F(BitArrayTest, operatorNotEqual) {
    EXPECT_EQ(true, (*bitArray) != (*toCompareBitArray));
}

TEST_F(BitArrayTest, swap) {
    toCompareBitArray->resize(35, true);
    BitArray oldArr = BitArray(*bitArray);
    toCompareBitArray->swap(*bitArray);
    EXPECT_EQ(oldArr.to_string(), toCompareBitArray->to_string());
    EXPECT_EQ(oldArr.size(), toCompareBitArray->size());
}

TEST_F(BitArrayTest, operatorAndEqual) {
    *toCompareBitArray &= *bitArray;
    EXPECT_EQ("0000", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, operatorErrorSize) {
    bitArray->resize(10);
    toCompareBitArray->resize(5);
    EXPECT_THROW(*bitArray &= *toCompareBitArray, std::invalid_argument);
    EXPECT_THROW(*bitArray |= *toCompareBitArray, std::invalid_argument);
    EXPECT_THROW(*bitArray ^= *toCompareBitArray, std::invalid_argument);
}

TEST_F(BitArrayTest, operatorErrorSizeTwo) {
    bitArray->resize(10);
    toCompareBitArray->resize(5);
    EXPECT_THROW(BitArray result = *bitArray & *toCompareBitArray, std::invalid_argument);
    EXPECT_THROW(BitArray result = *bitArray | *toCompareBitArray, std::invalid_argument);
    EXPECT_THROW(BitArray result = *bitArray ^ *toCompareBitArray, std::invalid_argument);
}

TEST_F(BitArrayTest, operatorOrEqual) {
    bitArray->set(1, true);
    bitArray->set(2, true);
    *toCompareBitArray |= *bitArray;
    EXPECT_EQ("0111", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, operatorXorEqual) {
    *toCompareBitArray ^= *bitArray;
    EXPECT_EQ("0001", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, operatorAnd) {
    BitArray newBitArray = (*toCompareBitArray) & (*bitArray);
    EXPECT_EQ("0000", newBitArray.to_string());
}

TEST_F(BitArrayTest, operatorOr) {
    BitArray newBitArray = (*toCompareBitArray) | (*bitArray);
    EXPECT_EQ("0001", newBitArray.to_string());
}

TEST_F(BitArrayTest, operatorXor) {
    BitArray newBitArray = (*toCompareBitArray) ^ (*bitArray);
    EXPECT_EQ("0001", newBitArray.to_string());
}

TEST_F(BitArrayTest, operatorLeftShiftEqual) {
    toCompareBitArray->set(1, true);
    *toCompareBitArray <<= 1;
    EXPECT_EQ("0110", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, operatorRightShiftEqual) {
    toCompareBitArray->set(1, true);
    *toCompareBitArray >>= 1;
    EXPECT_EQ("0001", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, operatorRightShift) {
    toCompareBitArray->set(1, true);
    *toCompareBitArray = *toCompareBitArray >> 1;
    EXPECT_EQ("0001", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, operatorLeftShift) {
    toCompareBitArray->set(1, true);
    *toCompareBitArray = *toCompareBitArray << 1;
    EXPECT_EQ("0110", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, pushBackResize) {
    toCompareBitArray->clear();
    toCompareBitArray->push_back(true);
    EXPECT_EQ("1", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, clear) {
    toCompareBitArray->clear();
    EXPECT_EQ("", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, setWithParam) {
    toCompareBitArray->set(2, true);
    EXPECT_EQ("0101", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, setAll) {
    toCompareBitArray->set();
    EXPECT_EQ("1111", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, resetAll) {
    toCompareBitArray->reset();
    EXPECT_EQ("0000", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, resetWithParam) {
    toCompareBitArray->reset(0);
    EXPECT_EQ("0000", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, resize) {
    toCompareBitArray->resize(9, true);
    EXPECT_EQ("111110001", toCompareBitArray->to_string());
}

TEST_F(BitArrayTest, resizeBig) {
    toCompareBitArray->resize(33, true);
    EXPECT_EQ(33, toCompareBitArray->size());
}

TEST_F(BitArrayTest, anyTrue) {
    EXPECT_EQ(true, toCompareBitArray->any());
}

TEST_F(BitArrayTest, anyFalse) {
    EXPECT_EQ(false, bitArray->any());
}

TEST_F(BitArrayTest, noneTrue) {
    EXPECT_EQ(true, bitArray->none());
}

TEST_F(BitArrayTest, noneFalse) {
    EXPECT_EQ(false, toCompareBitArray->none());
}

TEST_F(BitArrayTest, size) {
    EXPECT_EQ(4, bitArray->size());
}

TEST_F(BitArrayTest, countMethod) {
    EXPECT_EQ(bitArray->count(), 0);
    EXPECT_EQ(toCompareBitArray->count(), 1);
}

TEST_F(BitArrayTest, emptyFalse) {
    EXPECT_EQ(false, bitArray->empty());
}

TEST_F(BitArrayTest, emptyTrue) {
    bitArray->clear();
    EXPECT_EQ(true, bitArray->empty());
}

TEST_F(BitArrayTest, operatorTilda) {
    BitArray bitArray1 = BitArray(4, 3);
    bitArray1 = ~bitArray1;
    EXPECT_EQ("1100", bitArray1.to_string());
}
