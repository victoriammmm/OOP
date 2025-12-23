#pragma once

#include <string>
#include <list>

class FileReader {
public:
    bool readFile(const std::string& filename);
    std::list<std::string> getLines() const {
        return lines;
    }
private:
    std::list<std::string> lines;
};
