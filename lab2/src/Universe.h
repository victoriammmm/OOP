#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include "Field.h"

class Universe{
private:
    Field field;
    int iteration;
    std::string name;
    std::string rule;
    int countNeighbors(int x, int y) const;

public:
    Universe(int width, int height, const std::string& rule);
    void randomInit();
    void tick();
    const Field& getField() const;
    void initialize(const std::string& name, const std::string& rule, int width, int height);
    void setAliveCell(int x, int y);
    const std::string& getName() const;
    const std::string& getRule() const;
    int getIteration() const;
};