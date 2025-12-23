#include <gtest/gtest.h>
#include "Universe.h"

TEST(UniverseTest, Constructor) {
    Universe universe(10, 20, "B3/S23");
    EXPECT_EQ(universe.getWidth(), 10);
    EXPECT_EQ(universe.getHeight(), 20);
    EXPECT_EQ(universe.getRule(), "B3/S23");
    EXPECT_EQ(universe.getName(), "Unnamed Universe");
    EXPECT_EQ(universe.getIteration(), 0);
}

TEST(UniverseTest, Initialize) {
    Universe universe(5, 5, "B3/S23");
    universe.initialize("Test Universe", "B36/S23", 8, 8);
    EXPECT_EQ(universe.getName(), "Test Universe");
    EXPECT_EQ(universe.getRule(), "B36/S23");
    EXPECT_EQ(universe.getWidth(), 8);
    EXPECT_EQ(universe.getHeight(), 8);
    EXPECT_EQ(universe.getIteration(), 0);
}

TEST(UniverseTest, SetAliveCell) {
    Universe universe(5, 5, "B3/S23");
    universe.setAliveCell(2, 2);
    EXPECT_EQ(universe.getField().getCell(2, 2), CellState::Alive);
    EXPECT_EQ(universe.getField().getCell(0, 0), CellState::Dead);
}

TEST(UniverseTest, TickBlockPattern) {
    Universe universe(4, 4, "B3/S23");
    universe.setAliveCell(1, 1);
    universe.setAliveCell(1, 2);
    universe.setAliveCell(2, 1);
    universe.setAliveCell(2, 2);
    universe.tick();
    EXPECT_EQ(universe.getField().getCell(1, 1), CellState::Alive);
    EXPECT_EQ(universe.getField().getCell(1, 2), CellState::Alive);
    EXPECT_EQ(universe.getField().getCell(2, 1), CellState::Alive);
    EXPECT_EQ(universe.getField().getCell(2, 2), CellState::Alive);
    EXPECT_EQ(universe.getIteration(), 1);
}

TEST(UniverseTest, RandomInit) {
    Universe universe(10, 10, "B3/S23");
    universe.randomInit();
    bool hasAliveCell = false;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (universe.getField().getCell(i, j) == CellState::Alive) {
                hasAliveCell = true;
                break;
            }
        }
    }
    EXPECT_TRUE(hasAliveCell);
}