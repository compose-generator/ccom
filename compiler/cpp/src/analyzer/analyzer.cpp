// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 11.05.2021.
//

#include "analyzer.h"

json getJsonValueFromKey(const std::unique_ptr<KeyExprAST> &key, json data) {
    for (const std::unique_ptr<IdentifierExprAST>& identifier : key->getIdentifiers()) {
        std::string identifierName = identifier->getName();
        int identifierIndex = identifier->getIndex();
        if (!data.contains(identifierName))
            throw std::runtime_error("Identifier " + identifierName + " does not exist in data input");

        data = data[identifierName];

        if (identifierIndex >= 0) { // Identifier has an index attached to it
            if (data.empty())
                throw std::runtime_error("Index " + std::to_string(identifierIndex) + " does not exist in identifier " + identifierName);
            data = data[identifierIndex];
        }
    }
    return data;
}

// ------------------------------------------- Check Data Type Compatibility -------------------------------------------

void checkDataTypeCompatibility(bool isSingleStatement, TopLevelExprAST* ast, const json& data) {
    if (isSingleStatement) {
        if (ast->getType() != TopLevelExprAST::STMT_LST_EXPR)
            throw std::runtime_error("Input was no single statement list");
        auto* stmtLst = static_cast<StmtLstExprAST*>(ast);
        checkDataTypeCompatibilityStmtList(stmtLst, data);
    } else {
        auto* content = static_cast<ContentExprAST*>(ast);
        checkDataTypeCompatibilityContent(content, data);
    }
}

void checkDataTypeCompatibilityContent(ContentExprAST* content, const json& data) {
    // Loop through sections
    for (const std::unique_ptr<ContentBlockExprAST>& contentBlock : content->getContentBlocks()) {
        if (contentBlock->getType() == ContentBlockExprAST::SECTION_EXPR) {
            auto* relevantSection = static_cast<SectionExprAST*>(contentBlock.get());
            // Loop through comBlocks
            for (const std::unique_ptr<ComBlockExprAST>& comBlock : relevantSection->getComBlocks()) {
                switch (comBlock->getType()) {
                    case ComBlockExprAST::COM_LINE_BLOCK_EXPR: {
                        auto* lineBlockExpr = static_cast<ComLineBlockExprAST*>(comBlock.get());
                        checkDataTypeCompatibilityStmtList(lineBlockExpr->getStmtList().get(), data);
                        continue;
                    }
                    case ComBlockExprAST::COM_BLOCK_BLOCK_EXPR: {
                        auto* blockBlockExpr = static_cast<ComBlockBlockExprAST*>(comBlock.get());
                        checkDataTypeCompatibilityStmtList(blockBlockExpr->getIfBlock()->getStmtList().get(), data);
                        continue;
                    }
                    default:
                        throw std::runtime_error("Got unknown ComBlock object");
                }
            }
        }
    }
}

void checkDataTypeCompatibilityStmtList(StmtLstExprAST* stmtLst, const json& data) {
    // Loop through statements
    for (const std::unique_ptr<StmtExprAST>& stmt : stmtLst->getStatements()) {
        if (stmt->getType() == StmtExprAST::COMP_STMT_EXPR) {
            auto* compStmt = static_cast<CompStmtExprAST*>(stmt.get());
            checkDataTypeCompatibilityCompStmt(compStmt, data);
        }
    }
}

void checkDataTypeCompatibilityCompStmt(CompStmtExprAST* compStmt, const json& data) {
    // Check if 'value' has the same type as the JSON key value
    auto jsonKeyValue = getJsonValueFromKey(compStmt->getKey(), data);

    switch (compStmt->getValue()->getType()) {
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
            return;
        case ValueExprAST::VALUE_EXPR:
            throw std::runtime_error(jsonKeyValue.dump() + " is an unknown data type");
    }
}

TopLevelExprAST* executeSemanticAnalysis(bool isSingleStatement, const std::string& fileInput, const json& data,
                                 const std::string& lineCommentChars, const std::string& blockCommentCharsOpen,
                                 const std::string& blockCommentCharsClose) {
    // Parse Abstract Syntax Tree
    TopLevelExprAST* ast = executeSyntaxAnalysis(isSingleStatement, fileInput, lineCommentChars,
                                         blockCommentCharsOpen, blockCommentCharsClose);

    // Execute semantic checks
    checkDataTypeCompatibility(isSingleStatement, ast, data);

    return ast;
}