#include "Field.h"

Field::Field(int width, int height)
    : width(width), height(height), cells(width * height, CellState::Dead){
}

CellState Field::getCell(int x, int y) const{
    return cells[getIndex(x, y)];
}

void Field::setCell(int x, int y, CellState state){
    cells[getIndex(x, y)] = state;
}

int Field::getWidth() const{
    return width;
}

int Field::getHeight() const{
    return height;
}

void Field::resize(int newWidth, int newHeight){
    width = newWidth;
    height = newHeight;
    cells.resize(width * height, CellState::Dead);
}

int Field::getIndex(int x, int y) const{
    x = (x % width + width) % width;
    y = (y % height + height) % height;
    return x * height + y;
}