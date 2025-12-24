#pragma once
#include <string>
#include "Universe.h"

class UniverseLoader {
public:
    static void loadFromFile(Universe& universe, const std::string& filename);
};