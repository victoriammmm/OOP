#include "Universe.h"
#include <fstream>
#include <random>
#include "Rule.h"

Universe::Universe(int width, int height, const std::string& rule)
    : field(width, height), iteration(0), rule(rule){
}

void Universe::randomInit(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < field.getWidth(); ++i){
        for (int j = 0; j < field.getHeight(); ++j){
            if (dis(gen) == 1) {
                field.setCell(i, j, CellState::Alive);
            } else {
                field.setCell(i, j, CellState::Dead);
            }
        }
    }
}

void Universe::tick(){
    Field new_field = field;
    Rule rule_obj(rule);

    for (int i = 0; i < field.getWidth(); ++i){
        for (int j = 0; j < field.getHeight(); ++j){
            int neighbors = countNeighbors(i, j);
            bool isAlive = field.getCell(i, j) == CellState::Alive;
            if (rule_obj.shouldLive(isAlive, neighbors)){
                new_field.setCell(i, j, CellState::Alive);
            }
            else{
                new_field.setCell(i, j, CellState::Dead);
            }
        }
    }
    field = new_field;
    ++iteration;
}

const Field& Universe::getField() const{
    return field;
}

void Universe::initialize(const std::string& name, const std::string& rule,
                         int width, int height) {
    this->name = name;
    this->rule = rule;
    field.resize(width, height);
    iteration = 0;
}

void Universe::setAliveCell(int x, int y) {
    if (x >= 0 && x < field.getWidth() && y >= 0 && y < field.getHeight()) {
        field.setCell(x, y, CellState::Alive);
    }
}

const std::string& Universe::getName() const{
    return name;
}

const std::string& Universe::getRule() const{
    return rule;
}

int Universe::getIteration() const{
    return iteration;
}

int Universe::countNeighbors(int x, int y) const{
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx){ //влево-вправо
        for (int dy = -1; dy <= 1; ++dy){ //вверх-вниз
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + field.getWidth()) % field.getWidth();
            int ny = (y + dy + field.getHeight()) % field.getHeight();
            if (field.getCell(nx, ny) == CellState::Alive){
                ++count;
            }
        }
    }
    return count;
}