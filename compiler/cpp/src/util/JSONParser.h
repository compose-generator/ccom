//
// Created by Marc on 24.05.2021.
//

# pragma once

#include "../../lib/json/json.hpp"
#include "../parser/ast/KeyExprAST.h"

using json = nlohmann::json;

class JSONParser {
public:
    // Constructors
    explicit JSONParser(): data(json::parse("{}")) {}
    JSONParser(json data): data(std::move(data)) {}
    JSONParser(std::string dataString): data(json::parse(dataString)) {}

    // Public methods
    json getJSONValueFromKey(const std::unique_ptr<KeyExprAST>&);
    bool jsonKeyExists(const std::unique_ptr<KeyExprAST>&);
private:
    // Members
    json data;
};