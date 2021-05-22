// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 11.05.2021.
//

#include "analyzer.h"

json getJsonValueFromKey(const std::unique_ptr<KeyExprAST> &key, json data) {
    for (const std::unique_ptr<IdentifierExprAST>& identifier : key->GetIdentifiers()) {
        std::string identifierName = identifier->GetName();
        int identifierIndex = identifier->GetIndex();
        if (!data.contains(identifierName))
            throw std::runtime_error("Identifier " + identifierName + " does not exist in data input");

        data = data[identifierName];

        if (identifierIndex >= 0) { // Identifier has an index attached to it
            if (data.empty())
                throw std::runtime_error("index " + std::to_string(identifierIndex) + " does not exist in identifier " + identifierName);
            data = data[identifierIndex];
        }
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
            // Loop through comBlocks
            for (const std::unique_ptr<ComBlockExprAST>& comBlock : relevantSection->GetComBlocks()) {
                switch (comBlock->GetType()) {
                    case ComBlockExprAST::COM_LINE_BLOCK_EXPR: {
                        auto* lineBlockExpr = static_cast<ComLineBlockExprAST*>(comBlock.get());
                        checkDataTypeCompatibilityStmtList(lineBlockExpr->GetStmtList().get(), data);
                        continue;
                    }
                    case ComBlockExprAST::COM_BLOCK_BLOCK_EXPR: {
                        auto* blockBlockExpr = static_cast<ComBlockBlockExprAST*>(comBlock.get());
                        checkDataTypeCompatibilityStmtList(blockBlockExpr->GetIfBlock()->GetStmtList().get(), data);
                        continue;
                    }
                    default:
                        throw std::runtime_error("Got unknown ComBlock object");
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
    // Check if 'value' has the same type as the JSON key value
    auto jsonKeyValue = getJsonValueFromKey(compStmt->GetKey(), data);

    switch (compStmt->GetValue()->GetType()) {
        case ValueExprAST::STRING_EXPR:
            if (!jsonKeyValue.is_string())
                throw std::runtime_error(jsonKeyValue.dump() + " is not a string");
            return;
        case ValueExprAST::BOOLEAN_EXPR:
            if (!jsonKeyValue.is_boolean())
                throw std::runtime_error(jsonKeyValue.dump() + " is not a boolean");
            return;
        case ValueExprAST::NUMBER_EXPR:
            if (!jsonKeyValue.is_number_integer())
                throw std::runtime_error(jsonKeyValue.dump() + " is not an integer");
        case ValueExprAST::VALUE_EXPR:
            throw std::runtime_error(jsonKeyValue.dump() + " is an unknown data type");
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
