#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Converter.h"

template<typename Product_T, typename Id_T, typename Creator_T>
class Factory {
public:
    static Factory* getInstance() {
        static Factory f;
        return &f;
    }

    std::unique_ptr<Product_T> create(const Id_T& id) {
        auto it = creators.find(id);
        if (it == creators.end()) {
            return nullptr;
        }
        return std::unique_ptr<Product_T>(it->second());
    }

    bool registerCreator(const Id_T& id, Creator_T creator) {
        creators[id] = creator;
        return true;
    }

    std::vector<Id_T> getAvailableIds() const {
        std::vector<Id_T> ids;
        for (const auto& pair : creators) {
            ids.push_back(pair.first);
        }
        return ids;
    }

    bool hasCreator(const Id_T& id) const {
        return creators.find(id) != creators.end();
    }

private:
    std::map<Id_T, Creator_T> creators;
};

using ConverterFactory = Factory<IConverter, std::string, IConverter* (*)()>;