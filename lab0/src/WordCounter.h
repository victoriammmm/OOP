#pragma once

#include <string>
#include <map>
#include <list>
#include <vector>

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
