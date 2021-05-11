// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 11.05.2021.
//

#include "analyzer.h"

ExprAST* executeSemanticAnalysis(bool isSingleStatement, const std::string& fileInput, const std::string& dataInput,
                                 const std::string& lineCommentChars, const std::string& blockCommentCharsOpen,
                                 const std::string& blockCommentCharsClose) {
    ExprAST* ast = executeSyntaxAnalysis(isSingleStatement, fileInput, dataInput, lineCommentChars,
                                         blockCommentCharsOpen, blockCommentCharsClose);

    return ast;
}