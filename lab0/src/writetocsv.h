#pragma once

#include <string>
#include "wordcounter.h"

class CSVWriter {
public:
    static bool writeCSV(const std::string& filename, const WordCounter& counter);
};