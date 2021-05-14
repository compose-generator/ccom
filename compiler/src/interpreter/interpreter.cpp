// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 14.05.2021.
//

#include "interpreter.h"

bool getExistenceOfJsonKey(const std::unique_ptr<KeyExprAST> &key, json data) {
    for (const std::unique_ptr<IdentifierExprAST>& identifier : key->GetIdentifiers()) {
        std::string keyName = identifier->GetName();
        std::cout << keyName << std::endl;
        if (!data.contains(keyName)) return false;
        data = data[keyName];
    }
    return true;
}

std::string getOutput(bool isSingleStatement, ExprAST* ast, const json& data, bool preserveCommentsOnFalse) {
    if (isSingleStatement) {
        return evaluateStmtList(ast, data) ? "true" : "false";
    } else {
        return getOutputOfContent(ast, data, preserveCommentsOnFalse);
    }
}

std::string getOutputOfContent(ExprAST* ast, const json& data, bool preserveCommentsOnFalse) {
    std::string result;

    auto* content = dynamic_cast<ContentExprAST*>(ast);
    for (const std::unique_ptr<ExprAST>& section : content->GetSections()) {
        // Is section an arbitrary section?
        if (auto* arbitrarySection = dynamic_cast<ArbitraryExprAST*>(section.get())) {
            result += getOutputOfArbitrarySection(arbitrarySection);
        }
        // Is section a relevant section?
        if (auto* relevantSection = dynamic_cast<SectionExprAST*>(section.get())) {
            result += getOutputOfRelevantSection(relevantSection, data);
        }
    }
    return result;
}

std::string getOutputOfArbitrarySection(ArbitraryExprAST* arbitraryExpr) {
    return arbitraryExpr->GetValue();
}

std::string getOutputOfRelevantSection(SectionExprAST* relevantSection, const json& data) {
    std::string result;

    // Loop through com blocks
    for (const std::unique_ptr<ComBlockExprAST>& comBlock : relevantSection->GetComBlocks()) {
        if (auto* lineBlockExpr = dynamic_cast<ComLineBlockExprAST*>(comBlock.get())) {
            // Evaluate condition and append payload to output string if condition was truthy
            if (evaluateStmtList(lineBlockExpr->GetStmtList().get(), data)) {
                result += lineBlockExpr->GetPayload()->GetValue();
            }
        } else if(auto* blockBlockExpr = dynamic_cast<ComBlockBlockExprAST*>(comBlock.get())) {
            const std::unique_ptr<IfBlockExprAST>& ifBlock = blockBlockExpr->GetIfBlock();
            // Evaluate condition and append payload to output string if condition was truthy
            if (evaluateStmtList(ifBlock->GetStmtList().get(), data)) {
                result += ifBlock->GetPayload()->GetValue();
            }
        }
    }
    return result;
}

bool evaluateStmtList(ExprAST* ast, const json& data) {
    auto* stmtList = dynamic_cast<StmtLstExprAST*>(ast);
    // Loop through statements
    for (const std::unique_ptr<StmtExprAST>& stmt : stmtList->GetStatements()) {
        if (auto* hasStmt = dynamic_cast<HasStmtExprAST*>(stmt.get())) {
            return evaluateHasStatement(hasStmt, data);
        } else if (auto* compStmt = dynamic_cast<CompStmtExprAST*>(stmt.get())) {
            return evaluateCompStatement(compStmt, data);
        }
    }
    return false;
}

bool evaluateHasStatement(HasStmtExprAST* ast, const json& data) {
    std::cout << "HasStmt" << std::endl;
    bool isKeyExisting = getExistenceOfJsonKey(ast->GetKey(), data);
    if (ast->GetInverted()) return !isKeyExisting;
    return isKeyExisting;
}

bool evaluateCompStatement(CompStmtExprAST* ast, const json& data) {
    std::cout << "CompStmt" << std::endl;
    json keyValue = getJsonValueFromKey(ast->GetKey(), data);
    if (keyValue.is_string()) {
        auto value = keyValue.get<std::string>();
        if (auto* expectedValue = dynamic_cast<StringExprAST*>(ast->GetValue().get())) {
            return value == expectedValue->GetValue();
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - JSON value was string and hardcoded was not");
    } else if (keyValue.is_number_integer()) {
        auto value = keyValue.get<int>();
        if (auto* expectedValue = dynamic_cast<NumberExprAST*>(ast->GetValue().get())) {
            return value == expectedValue->GetValue();
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - JSON value was int and hardcoded was not");
    }
    throw std::runtime_error("Unknown datatype of '" + keyValue.dump() + "'");
}

std::string interpretInput(bool isSingleStatement, bool preserveCommentsOnFalse, const std::string& fileInput,
                           const std::string& dataInput, const std::string& lineCommentChars,
                           const std::string& blockCommentCharsOpen, const std::string& blockCommentCharsClose) {
    // Parse input string from JSON to object
    json data = json::parse(dataInput);

    // Get semantically checked AST
    ExprAST* ast = executeSemanticAnalysis(isSingleStatement, fileInput, data, lineCommentChars,
                                           blockCommentCharsOpen, blockCommentCharsClose);

    std::string output = getOutput(isSingleStatement, ast, data, preserveCommentsOnFalse);

    std::cout << "Output: " << output << std::endl;
}