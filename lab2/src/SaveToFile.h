#pragma once
#include <string>

class Universe;
class FileView{
public:
    void saveToFile(const Universe& universe, const std::string& filename);
};