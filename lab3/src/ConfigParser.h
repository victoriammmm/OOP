#pragma once

#include <string>
#include <vector>

struct ConfigCommand {
    std::string name;
    std::vector<std::string> parameters;
};

class ConfigParser {
private:
    std::string configFile;

    std::vector<std::string> tokenize(const std::string& line) const;
    bool isComment(const std::string& line) const;
    void validateCommand(const ConfigCommand& cmd, int lineNum) const;

public:
    explicit ConfigParser(const std::string& configFile)
        : configFile(configFile) {}

    std::vector<ConfigCommand> parse() const;
};