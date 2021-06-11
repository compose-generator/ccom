// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

#include <string>
#include <stdexcept>
#include "../../lib/json/json.hpp"
#include "../parser/Parser.h"
#include "../util/JSONParser.h"

using json = nlohmann::json;

class Analyzer {
public:
    // Constructors
    explicit Analyzer() {}
    Analyzer(bool, const std::string&, JSONParser, const std::string&, const std::string&, const std::string&);

    // Public methods
    TopLevelExprAST* getAST();
    void executeAnalysis();
private:
    // Private methods
    void checkDataTypeCompatibility();
    void checkDataTypeCompatibilityContent(ContentExprAST*);
    void checkDataTypeCompatibilityStmtList(StmtLstExprAST*);
    void checkDataTypeCompatibilityCompStmt(CompStmtExprAST*);

    // Members
    Parser parser;
    JSONParser jsonParser;
    TopLevelExprAST* ast = nullptr;
    bool isSingleStatement = false;
};
