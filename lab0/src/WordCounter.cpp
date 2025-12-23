#include "WordCounter.h"
#include <iostream>
#include <algorithm>

void WordCounter::countWords(const std::list<std::string>& lines) {
    wordFrequency.clear();
    totalWords = 0;
    for (const auto& line : lines) {
        std::string currentWord;
        for (const unsigned char character : line) {
            if (std::isalnum(character)) {
                currentWord += static_cast<char>(std::tolower(character));
            }
            else {
                if (!currentWord.empty()) {
                    wordFrequency[currentWord]++;
                    totalWords++;
                    currentWord.clear();
                }
            }
        }
        if (!currentWord.empty()) {
            wordFrequency[currentWord]++;
            totalWords++;
        }
    }
}

void WordCounter::calculateFrequency() {
    sortedWords.clear();
    for (const auto& pair : wordFrequency) {
        if (!pair.first.empty()) {
            double percentage = (static_cast<double>(pair.second) / totalWords) * 100.0;
            sortedWords.push_back({pair.first, {pair.second, percentage}});
        }
    }
}

void WordCounter::sortByFrequency() {
    auto compareByFrequencyDescending = [](const auto& first, const auto& second) {
        int firstFrequency = first.second.first;
        int secondFrequency = second.second.first;
        return firstFrequency > secondFrequency;
    };
    std::ranges::sort(sortedWords, compareByFrequencyDescending);
}


