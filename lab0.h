#pragma once

#include <string>
#include <map>
#include <list>
#include <vector>


class FileReader {
public:
    bool readFile(const std::string& filename);
    std::list<std::string> getLines() const {
        return lines;
    }
private:
    std::list<std::string> lines;
};

class WordCounter {
public:
    void countWords(const std::list<std::string>& lines);
    void calculateFrequency();
    void sortByFrequency();
    std::vector<std::pair<std::string, std::pair<int, double>>> getSortedWords() const {
        return sortedWords;
    }
    int getTotalWords() const {
        return totalWords;
    }
private:
    std::map<std::string, int> wordFrequency;
    std::vector<std::pair<std::string, std::pair<int, double>>> sortedWords;
    int totalWords = 0;
};

class CSVWriter {
public:
    static bool writeCSV(const std::string& filename, const WordCounter& counter);
};