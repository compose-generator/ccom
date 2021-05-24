//
// Created by Marc on 24.05.2021.
//

#pragma once

#include "../analyzer/Analyzer.h"

class Interpreter {
private:
    // Constant inputs
    Analyzer analyzer;
    JSONParser jsonParser;
    TopLevelExprAST* ast{};
    bool isSingleStatement;

    // Private functions
    std::string getOutput();
    std::string getOutputOfContent(ContentExprAST*);
    std::string getOutputOfArbitrarySection(ArbitraryExprAST*);
    std::string getOutputOfRelevantSection(SectionExprAST*);

    bool evaluateStmtList(StmtLstExprAST*);
    bool evaluateHasStatement(HasStmtExprAST*);
    bool evaluateCompStatement(CompStmtExprAST*);
    template <typename T> bool evaluateCondition(T leftValue, T rightValue, Operator op);
public:
    explicit Interpreter() {}
    Interpreter(bool, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
    std::string interpretInput();
};
