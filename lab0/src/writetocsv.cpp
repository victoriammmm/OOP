#include "writetocsv.h"
#include <fstream>
#include <iomanip>

bool CSVWriter::writeCSV(const std::string& filename, const WordCounter& counter) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file << "Word,Frequency,Frequency(%)\n";
    auto sortedWords = counter.getSortedWords();
    for (const auto& wordData : sortedWords) {
        file << wordData.first << ","
             << wordData.second.first << ","
             << std::fixed << std::setprecision(2) << wordData.second.second << "\n";
    }
    file.close();
    return true;
}

