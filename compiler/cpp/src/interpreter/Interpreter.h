// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

#include "../analyzer/Analyzer.h"
#include <stdexcept>

class Interpreter {
public:
    // Constructors
    explicit Interpreter() {}
    Interpreter(bool, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

    // Public methods
    std::string interpretInput();
private:
    // Private methods
    std::string getOutput();
    std::string getOutputOfContent(ContentExprAST*);
    std::string getOutputOfArbitrarySection(ArbitraryExprAST*);
    std::string getOutputOfRelevantSection(SectionExprAST*);
    bool evaluateStmtList(StmtLstExprAST*);
    bool evaluateHasStatement(HasStmtExprAST*);
    bool evaluateCompStatement(CompStmtExprAST*);
    template <typename T> bool evaluateCondition(T leftValue, T rightValue, Operator op);

    // Members
    Analyzer analyzer;
    JSONParser jsonParser;
    TopLevelExprAST* ast{};
    bool isSingleStatement;
};
