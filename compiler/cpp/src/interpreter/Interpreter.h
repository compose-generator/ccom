/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include "../analyzer/Analyzer.h"
#include <stdexcept>
#include "../exception/UnexpectedDataTypeException.h"

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
    bool evaluateContainsStatement(ContainsStmtExprAST*);
    bool compareJsonWithValue(json&, ValueExprAST*, Operator);
    template <typename T> bool evaluateCondition(T, T, Operator);
    bool hasSuffix(const std::string&, const std::string&);

    // Members
    Analyzer analyzer;
    JSONParser jsonParser;
    TopLevelExprAST* ast{};
    bool isSingleStatement;
};
