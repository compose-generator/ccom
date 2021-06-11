// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "JSONParser.h"

#include <utility>

json JSONParser::getJSONValueFromKey(const std::unique_ptr<KeyExprAST>& key) {
    return getJSONValueFromKey(data, key);
}

json JSONParser::getJSONValueFromKey(const json data, const std::unique_ptr<KeyExprAST>& key) {
    json dataTmp = data;
    for (const std::unique_ptr<IdentifierExprAST>& identifier : key->getIdentifiers()) {
        std::string identifierName = identifier->getName();
        int identifierIndex = identifier->getIndex();
        if (!dataTmp.contains(identifierName))
            throw std::runtime_error("Identifier " + identifierName + " does not exist in data input");

        dataTmp = dataTmp[identifierName];

        if (identifierIndex >= 0) { // Identifier has an index attached to it
            if (dataTmp.empty())
                throw std::runtime_error("Index " + std::to_string(identifierIndex) + " does not exist in identifier " + identifierName);
            dataTmp = dataTmp[identifierIndex];
        }
    }
    return dataTmp;
}

bool JSONParser::jsonKeyExists(const std::unique_ptr<KeyExprAST> &key) {
    return jsonKeyExists(data, key);
}

bool JSONParser::jsonKeyExists(const json data, const std::unique_ptr<KeyExprAST> &key) {
    json dataTmp = data;
    for (const std::unique_ptr<IdentifierExprAST>& identifier : key->getIdentifiers()) {
        std::string identifierName = identifier->getName();
        int identifierIndex = identifier->getIndex();

        if (!dataTmp.contains(identifierName)) return false;
        dataTmp = dataTmp[identifierName];

        if (identifierIndex >= 0) { // Identifier has an index attached to it
            if (dataTmp.size() <= identifierIndex) return false;
            dataTmp = dataTmp[identifierIndex];
        }
    }
    return true;
}