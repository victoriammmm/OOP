#include <iostream>
#include "lab0.h"

int main(const int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "incorrect input";
        return 1;
    }
    const std::string inputFile = argv[1];
    const std::string outputFile = argv[2];
    FileReader reader;
    if (!reader.readFile(inputFile)) {
        std::cerr << "file reading error";
        return 1;
    }
    WordCounter counter;
    counter.countWords(reader.getLines());
    counter.calculateFrequency();
    counter.sortByFrequency();
    if (!CSVWriter::writeCSV(outputFile, counter)) {
        std::cerr << "file writing error";
        return 1;
    }
    return 0;
}
