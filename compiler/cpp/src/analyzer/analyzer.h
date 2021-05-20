// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 11.05.2021.
//

#pragma once

#include <nlohmann/json.hpp>
#include "../parser/ast/ExprAST.h"
#include "../parser/parser.h"

using json = nlohmann::json;

ExprAST* executeSemanticAnalysis(bool isSingleStatement, const std::string &fileInput, const json& data,
                               const std::string &lineCommentChars, const std::string &blockCommentCharsOpen,
                               const std::string &blockCommentCharsClose);

json getJsonValueFromKey(const std::unique_ptr<KeyExprAST>&, json);
void checkDataTypeCompatibility(bool, ExprAST*, const json&);
void checkDataTypeCompatibilityContent(ExprAST*, const json&);
void checkDataTypeCompatibilityStmtList(ExprAST*, const json&);
void checkDataTypeCompatibilityCompStmt(CompStmtExprAST*, const json&);
