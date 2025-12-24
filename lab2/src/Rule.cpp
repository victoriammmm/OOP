#include "Rule.h"
#include <stdexcept>

Rule::Rule(const std::string& rule){
    size_t slashPos = rule.find('/');
    if (slashPos == std::string::npos){
        throw std::invalid_argument(
            "Error: Invalid rule format. Expected 'Bx/Sy'.");
    }

    std::string birthStr = rule.substr(1, slashPos - 1);
    std::string survivalStr = rule.substr(slashPos + 2);

    for (char c : birthStr){
        if (isdigit(c)){
            birth.insert(c - '0');
        }
    }

    for (char c : survivalStr){
        if (isdigit(c)){
            survival.insert(c - '0');
        }
    }
}

bool Rule::shouldLive(bool isAlive, int neighbors) const{
    if (isAlive){
        return survival.count(neighbors) > 0;
    }
    else{
        return birth.count(neighbors) > 0;
    }
}