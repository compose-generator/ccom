// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 11.05.2021.
//

#include "analyzer.h"

void checkDataTypeCompatibility(ExprAST* ast, json data) {

}

ExprAST* executeSemanticAnalysis(bool isSingleStatement, const std::string& fileInput, const std::string& dataInput,
                                 const std::string& lineCommentChars, const std::string& blockCommentCharsOpen,
                                 const std::string& blockCommentCharsClose) {
    // Parse Abstract Syntax Tree
    ExprAST* ast = executeSyntaxAnalysis(isSingleStatement, fileInput, dataInput, lineCommentChars,
                                         blockCommentCharsOpen, blockCommentCharsClose);

    // Parse input string from JSON to object
    json data = json::parse(dataInput);

    // Execute semantic checks
    checkDataTypeCompatibility(ast, data);

    return ast;
}