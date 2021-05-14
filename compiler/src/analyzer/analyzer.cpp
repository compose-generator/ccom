// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 11.05.2021.
//

#include "analyzer.h"

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

json getJsonValueFromKey(const std::unique_ptr<KeyExprAST> &key, json data) {
    for (const std::unique_ptr<IdentifierExprAST>& identifier : key->GetIdentifiers()) {
        data = data[identifier->GetName()];
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
    for (const std::unique_ptr<SectionExprAST>& section : content->GetRelevantSections()) {
        for (const std::unique_ptr<ComBlockExprAST>& comBlock : section->GetComBlocks()) {
            if (auto* lineBlockExpr = dynamic_cast<ComLineBlockExprAST*>(comBlock.get())) {
                checkDataTypeCompatibilityStmtList(lineBlockExpr->GetStmtList().get(), data);
            } else if(auto* blockBlockExpr = dynamic_cast<ComBlockBlockExprAST*>(comBlock.get())) {
                checkDataTypeCompatibilityStmtList(blockBlockExpr->GetIfBlock()->GetStmtList().get(), data);
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

    if (auto* stringExpr = dynamic_cast<StringExprAST*>(compStmt->GetValue().get())) {
        if (!jsonKeyValue.is_string())
            throw std::runtime_error(jsonKeyValue.dump() + " is not a string");
    } else {
        if (!jsonKeyValue.is_number_integer())
            throw std::runtime_error(jsonKeyValue.dump() + " is not an integer");
    }
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
    checkDataTypeCompatibility(isSingleStatement, ast, data);

    return ast;
}
