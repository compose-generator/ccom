// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 14.05.2021.
//

#pragma once

#include <string>
#include "../parser/ast/ExprAST.h"
#include "../analyzer/analyzer.h"

using json = nlohmann::json;

std::string interpretInput(bool, const std::string&, const std::string&, const std::string&,
                         const std::string&, const std::string&);

std::string getOutput(bool, TopLevelExprAST*, const json&);
std::string getOutputOfContent(ContentExprAST*, const json&);
std::string getOutputOfArbitrarySection(ArbitraryExprAST*);
std::string getOutputOfRelevantSection(SectionExprAST*, const json&);
bool evaluateStmtList(StmtLstExprAST *ast, const json &data);
bool evaluateHasStatement(HasStmtExprAST*, const json&);
bool evaluateCompStatement(CompStmtExprAST*, const json&);