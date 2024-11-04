/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#include "JSONParser.h"

#include <utility>

json JSONParser::getJSONValueFromKey(const std::unique_ptr<ASTKeyExprNode> &key) { return getJSONValueFromKey(data, key); }

json JSONParser::getJSONValueFromKey(const json &data, const std::unique_ptr<ASTKeyExprNode> &key) {
  json dataTmp = data;
  for (const std::unique_ptr<ASTIdentifierExprNode> &identifier : key->identifiers) {
    const std::string identifierName = identifier->name;
    if (!dataTmp.contains(identifierName))
      throw std::runtime_error("Identifier " + identifierName + " does not exist in data input");

    dataTmp = dataTmp[identifierName];

    int identifierIndex = identifier->index;
    if (identifierIndex >= 0) { // Identifier has an index attached to it
      if (dataTmp.empty())
        throw std::runtime_error("Index " + std::to_string(identifierIndex) + " does not exist in identifier " + identifierName);
      dataTmp = dataTmp[identifierIndex];
    }
  }
  return dataTmp;
}

bool JSONParser::jsonKeyExists(const std::unique_ptr<ASTKeyExprNode> &key) { return jsonKeyExists(data, key); }

bool JSONParser::jsonKeyExists(const json &data, const std::unique_ptr<ASTKeyExprNode> &key) {
  json dataTmp = data;
  for (const std::unique_ptr<ASTIdentifierExprNode> &identifier : key->identifiers) {
    const std::string identifierName = identifier->name;
    int identifierIndex = identifier->index;

    if (!dataTmp.contains(identifierName))
      return false;
    dataTmp = dataTmp[identifierName];

    if (identifierIndex >= 0) { // Identifier has an index attached to it
      if (dataTmp.size() <= identifierIndex)
        return false;
      dataTmp = dataTmp[identifierIndex];
    }
  }
  return true;
}