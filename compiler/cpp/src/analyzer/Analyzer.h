//
// Created by Marc on 24.05.2021.
//

#pragma once

#include <string>
#include <stdexcept>
#include "../../lib/json/json.hpp"
#include "../parser/Parser.h"
#include "../util/JSONParser.h"

using json = nlohmann::json;

class Analyzer {
private:
    // Constant inputs
    Parser parser;
    JSONParser jsonParser;
    TopLevelExprAST* ast{};
    bool isSingleStatement = false;

    // Private functions
    void checkDataTypeCompatibility();
    void checkDataTypeCompatibilityContent(ContentExprAST*);
    void checkDataTypeCompatibilityStmtList(StmtLstExprAST*);
    void checkDataTypeCompatibilityCompStmt(CompStmtExprAST*);
public:
    explicit Analyzer() {}
    Analyzer(bool, const std::string&, JSONParser, const std::string&, const std::string&, const std::string&);
    TopLevelExprAST* getAST();
    void executeAnalysis();
};
