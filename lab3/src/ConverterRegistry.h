#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class ConverterRegistry {
private:
    struct ConverterInfo {
        std::string description;
        std::string usage;
    };
    static std::unordered_map<std::string, ConverterInfo> converterInfos;

public:
    static void registerConverter(const std::string& name, const std::string& description,
    const std::string& usage);
    static std::vector<std::string> getAvailableConverters();
    static std::string getDescription(const std::string& id);
    static std::string getUsage(const std::string& id);
    static bool hasConverter(const std::string& id);
};