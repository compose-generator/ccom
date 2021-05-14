// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 14.05.2021.
//

#ifndef COMPILER_INTERPRETER_H
#define COMPILER_INTERPRETER_H

#include <string>
#include "../parser/ast/ExprAST.h"
#include "../analyzer/analyzer.h"

using json = nlohmann::json;

std::string interpretInput(bool, bool, const std::string&, const std::string&, const std::string&,
                         const std::string&, const std::string&);

std::string getOutput(bool, ExprAST*, const json&, bool);
std::string getOutputOfContent(ExprAST*, const json&, bool);
std::string getOutputOfArbitrarySection(ArbitraryExprAST*);
std::string getOutputOfRelevantSection(SectionExprAST*, const json&);
bool evaluateStmtList(ExprAST *ast, const json &data);
bool evaluateHasStatement(HasStmtExprAST*, const json&);
bool evaluateCompStatement(CompStmtExprAST*, const json&);

#endif //COMPILER_INTERPRETER_H
