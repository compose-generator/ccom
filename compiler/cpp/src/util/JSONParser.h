//
// Created by Marc on 24.05.2021.
//

# pragma once

#include "../../lib/json/json.hpp"
#include "../parser/ast/KeyExprAST.h"

using json = nlohmann::json;

class JSONParser {
private:
    json data;
public:
    explicit JSONParser(): data(json::parse("{}")) {}
    JSONParser(json data): data(std::move(data)) {}
    JSONParser(std::string dataString): data(json::parse(dataString)) {}
    json getJSONValueFromKey(const std::unique_ptr<KeyExprAST>&);
    bool jsonKeyExists(const std::unique_ptr<KeyExprAST>&);
};