#include "LoadFromFile.h"
#include "Universe.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

void UniverseLoader::loadFromFile(Universe& universe, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open file.");
    }

    std::string line;
    std::vector<std::pair<int, int>> aliveCells;

    std::getline(file, line);
    if (line != "Life 1.06") {
        throw std::runtime_error("Error: Invalid file format. Expected 'Life 1.06'.");
    }

    std::getline(file, line);
    if (line.rfind("#N ", 0) != 0) {
        throw std::runtime_error("Error: Missing or invalid universe name.");
    }
    std::string name = line.substr(3);

    std::getline(file, line);
    if (line.rfind("#R ", 0) != 0) {
        throw std::runtime_error("Error: Missing or invalid rule.");
    }
    std::string rule = line.substr(3);

    std::getline(file, line);
    int width = 20, height = 20;
    if (line.rfind("#F ", 0) == 0) {
        std::string sizeStr = line.substr(3);
        size_t slashPos = sizeStr.find('/');
        if (slashPos != std::string::npos) {
            width = std::stoi(sizeStr.substr(0, slashPos));
            height = std::stoi(sizeStr.substr(slashPos + 1));
        }
        std::getline(file, line);
    }

    std::istringstream iss(line);
    int x, y;
    if (iss >> x >> y) {
        aliveCells.emplace_back(x, y);
    }

    while (file >> x >> y) {
        aliveCells.emplace_back(x, y);
    }

    file.close();

    universe.initialize(name, rule, width, height);

    for (const auto& cell : aliveCells) {
        universe.setAliveCell(cell.first, cell.second);
    }
}