// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 11.05.2021.
//

#include "analyzer.h"

json getJsonValueFromKey(const std::unique_ptr<KeyExprAST> &key, json data) {
    for (const std::unique_ptr<IdentifierExprAST>& identifier : key->GetIdentifiers()) {
        std::string keyName = identifier->GetName();
        if (!data.contains(keyName)) throw std::runtime_error("Key " + keyName + " does not exist in data input");
        data = data[keyName];
    }
    return data;
}

// ------------------------------------------- Check Data Type Compatibility -------------------------------------------

void checkDataTypeCompatibility(bool isSingleStatement, ExprAST* ast, const json& data) {
    if (isSingleStatement) {
        checkDataTypeCompatibilityStmtList(ast, data);
    } else {
        checkDataTypeCompatibilityContent(ast, data);
    }
}

void checkDataTypeCompatibilityContent(ExprAST* ast, const json& data) {
    auto* content = dynamic_cast<ContentExprAST*>(ast);
    // Loop through sections
    for (const std::unique_ptr<ExprAST>& section : content->GetSections()) {
        if (auto* relevantSection = dynamic_cast<SectionExprAST*>(section.get())) {
            // Loop through ComBlocks
            for (const std::unique_ptr<ComBlockExprAST>& comBlock : relevantSection->GetComBlocks()) {
                if (auto* lineBlockExpr = dynamic_cast<ComLineBlockExprAST*>(comBlock.get())) {
                    checkDataTypeCompatibilityStmtList(lineBlockExpr->GetStmtList().get(), data);
                } else if(auto* blockBlockExpr = dynamic_cast<ComBlockBlockExprAST*>(comBlock.get())) {
                    checkDataTypeCompatibilityStmtList(blockBlockExpr->GetIfBlock()->GetStmtList().get(), data);
                }
            }
        }
    }
}

void checkDataTypeCompatibilityStmtList(ExprAST* ast, const json& data) {
    auto* stmtList = dynamic_cast<StmtLstExprAST*>(ast);
    // Loop through statements
    for (const std::unique_ptr<StmtExprAST>& stmt : stmtList->GetStatements()) {
        if (auto* compStmt = dynamic_cast<CompStmtExprAST*>(stmt.get())) {
            checkDataTypeCompatibilityCompStmt(compStmt, data);
        }
    }
}

void checkDataTypeCompatibilityCompStmt(CompStmtExprAST* compStmt, const json& data) {
    // Check if 'Value' has the same type as the JSON key value
    auto jsonKeyValue = getJsonValueFromKey(compStmt->GetKey(), data);

    if (dynamic_cast<StringExprAST*>(compStmt->GetValue().get())) {
        if (!jsonKeyValue.is_string())
            throw std::runtime_error(jsonKeyValue.dump() + " is not a string");
    } else {
        if (!jsonKeyValue.is_number_integer())
            throw std::runtime_error(jsonKeyValue.dump() + " is not an integer");
    }
}

ExprAST* executeSemanticAnalysis(bool isSingleStatement, const std::string& fileInput, const json& data,
                                 const std::string& lineCommentChars, const std::string& blockCommentCharsOpen,
                                 const std::string& blockCommentCharsClose) {
    // Parse Abstract Syntax Tree
    ExprAST* ast = executeSyntaxAnalysis(isSingleStatement, fileInput, lineCommentChars,
                                         blockCommentCharsOpen, blockCommentCharsClose);

    // Execute semantic checks
    checkDataTypeCompatibility(isSingleStatement, ast, data);

    return ast;
}
