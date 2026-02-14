#include "ConverterRegistry.h"

std::unordered_map<std::string, ConverterRegistry::ConverterInfo> ConverterRegistry::converterInfos;

void ConverterRegistry::registerConverter(const std::string& name,
                                         const std::string& description,
                                         const std::string& usage) {
    converterInfos[name] = {description, usage};
}

std::vector<std::string> ConverterRegistry::getAvailableConverters() {
    std::vector<std::string> result;
    for (const auto& pair : converterInfos) {
        result.push_back(pair.first);
    }
    return result;
}

std::string ConverterRegistry::getDescription(const std::string& id) {
    auto it = converterInfos.find(id);
    if (it != converterInfos.end()) {
        return it->second.description;
    }
    return "No description available";
}

std::string ConverterRegistry::getUsage(const std::string& id) {
    auto it = converterInfos.find(id);
    if (it != converterInfos.end()) {
        return it->second.usage;
    }
    return "No usage information available";
}

bool ConverterRegistry::hasConverter(const std::string& id) {
    return converterInfos.find(id) != converterInfos.end();
}