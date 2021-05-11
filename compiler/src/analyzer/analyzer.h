// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 11.05.2021.
//

#ifndef COMPILER_ANALYZER_H
#define COMPILER_ANALYZER_H

#include <nlohmann/json.hpp>
#include "../parser/ast/ExprAST.h"
#include "../parser/parser.h"

using json = nlohmann::json;

ExprAST* executeSemanticAnalysis(bool isSingleStatement, const std::string &fileInput, const std::string &dataInput,
                               const std::string &lineCommentChars, const std::string &blockCommentCharsOpen,
                               const std::string &blockCommentCharsClose);

#endif //COMPILER_ANALYZER_H
