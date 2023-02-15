/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <parser/ASTNodes.h>

#include "../../lib/json/json.hpp"

using json = nlohmann::json;

class JSONParser {
public:
  // Constructors
  explicit JSONParser() : data(json::parse("{}")) {}
  explicit JSONParser(const std::string &dataString) : data(json::parse(dataString)) {}

  // Public methods
  json getJSONValueFromKey(const std::unique_ptr<ASTKeyExprNode> &);
  static json getJSONValueFromKey(const json &data, const std::unique_ptr<ASTKeyExprNode> &);
  bool jsonKeyExists(const std::unique_ptr<ASTKeyExprNode> &);
  static bool jsonKeyExists(const json &data, const std::unique_ptr<ASTKeyExprNode> &);

private:
  // Members
  json data;
};