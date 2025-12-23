#pragma once
#include <string>
#include <set>

class Rule{
public:
    Rule(const std::string& rule);
    bool shouldLive(bool isAlive, int neighbors) const;

private:
    std::set<int> birth;
    std::set<int> survival;
};