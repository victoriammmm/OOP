#include "CSVParser.h"
#include "TuplePrinter.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc != 2 and argc != 5) {
        std::cerr << "Default settings: <filename.csv>\n";
        std::cerr << "or custom settings: <filename.csv> <skipLines> <delimiter> <escapeChar>";
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input.is_open()) {
        std::cerr << "Cannot open file: " << argv[1] << "\n";
        return 1;
    }

    std::size_t skip = 1;// default settings
    char delimiter = ',';
    char quote = '"';

    if (argc == 5) {//custom settings
        skip = std::stoi(argv[2]);
        delimiter = argv[3][0];
        quote = argv[4][0];
    }

    try {
        CsvReader<int, std::string, int> reader(input, skip, delimiter, '\n', quote);
        for (const auto& record : reader) {
            std::cout << record << std::endl;
        }
    }

    catch (const CsvReadException& ex) {
        std::cerr << "[CSV ERROR] " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
