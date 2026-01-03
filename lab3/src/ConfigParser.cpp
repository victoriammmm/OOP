#include "ConfigParser.h"
#include "Exceptions.h"
#include "ConverterRegistry.h"
#include <fstream>
#include <sstream>
#include <cctype>

std::vector<std::string> ConfigParser::tokenize(const std::string& line) const {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;

    while (iss >> token) {
        if (!token.empty() && token.back() == ',') {
            token.pop_back();
        }
        tokens.push_back(token);
    }

    return tokens;
}

bool ConfigParser::isComment(const std::string& line) const {
    size_t firstChar = line.find_first_not_of(" \t");
    if (firstChar == std::string::npos) {
        return true;
    }
    return line[firstChar] == '#';
}

void ConfigParser::validateCommand(const ConfigCommand& cmd, int lineNum) const {
    if (!ConverterRegistry::hasConverter(cmd.name)) {
        throw ConfigException("Line " + std::to_string(lineNum) +
                            ": Unknown converter '" + cmd.name + "'");
    }

    if (cmd.name == "mute" && cmd.parameters.size() != 2) {
        throw ConfigException("Line " + std::to_string(lineNum) +
                            ": 'mute' requires 2 parameters (start end)");
    }
    else if (cmd.name == "mix" && cmd.parameters.size() != 2) {
        throw ConfigException("Line " + std::to_string(lineNum) +
                            ": 'mix' requires 2 parameters ($file start)");
    }
    else if (cmd.name == "reverse" && cmd.parameters.size() != 2) {
        throw ConfigException("Line " + std::to_string(lineNum) +
                            ": 'reverse' requires 2 parameters (start end)");
    }
}

std::vector<ConfigCommand> ConfigParser::parse() const {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        throw ConfigException("Cannot open config file: " + configFile);
    }

    std::vector<ConfigCommand> commands;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;

        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty() || isComment(line)) {
            continue;
        }

        auto tokens = tokenize(line);
        if (tokens.empty()) {
            continue;
        }

        ConfigCommand cmd;
        cmd.name = tokens[0];

        for (size_t i = 1; i < tokens.size(); ++i) {
            cmd.parameters.push_back(tokens[i]);
        }

        validateCommand(cmd, lineNumber);
        
        commands.push_back(cmd);
    }
    
    if (commands.empty()) {
        throw ConfigException("Config file is empty or contains only comments");
    }
    
    return commands;
}