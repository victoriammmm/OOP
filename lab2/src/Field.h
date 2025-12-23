#pragma once
#include <string>
#include <vector>

enum class CellState{
    Dead,
    Alive
};

class Field{
public:
    Field(int width, int height);
    CellState getCell(int x, int y) const;
    void setCell(int x, int y, CellState state);
    int getWidth() const;
    int getHeight() const;
    void resize(int newWidth, int newHeight);

private:
    int width;
    int height;
    std::vector<CellState> cells;

    int getIndex(int x, int y) const;
};