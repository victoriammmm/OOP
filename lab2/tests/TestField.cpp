#include <gtest/gtest.h>
#include "Field.h"

TEST(FieldTest, ConstructorAndGetters) {
    Field field(10, 20);
    EXPECT_EQ(field.getWidth(), 10);
    EXPECT_EQ(field.getHeight(), 20);
}

TEST(FieldTest, SetAndGetCell) {
    Field field(5, 5);
    EXPECT_EQ(field.getCell(0, 0), CellState::Dead);
    field.setCell(0, 0, CellState::Alive);
    EXPECT_EQ(field.getCell(0, 0), CellState::Alive);
    field.setCell(0, 0, CellState::Dead);
    EXPECT_EQ(field.getCell(0, 0), CellState::Dead);
}

TEST(FieldTest, ToroidalWrapping) {
    Field field(3, 3);
    field.setCell(5, 5, CellState::Alive);
    EXPECT_EQ(field.getCell(2, 2), CellState::Alive);
    field.setCell(-1, -1, CellState::Alive);
    EXPECT_EQ(field.getCell(2, 2), CellState::Alive);
}

TEST(FieldTest, Resize) {
    Field field(2, 2);
    field.setCell(0, 0, CellState::Alive);
    field.setCell(1, 1, CellState::Alive);
    field.resize(4, 4);
    EXPECT_EQ(field.getWidth(), 4);
    EXPECT_EQ(field.getHeight(), 4);
    EXPECT_EQ(field.getCell(0, 0), CellState::Alive);
    EXPECT_EQ(field.getCell(1, 1), CellState::Alive);
    EXPECT_EQ(field.getCell(3, 3), CellState::Dead);
}
