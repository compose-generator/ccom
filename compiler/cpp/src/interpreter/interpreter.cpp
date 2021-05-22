// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 14.05.2021.
//

#include "interpreter.h"

bool getExistenceOfJsonKey(const std::unique_ptr<KeyExprAST> &key, json data) {
    for (const std::unique_ptr<IdentifierExprAST>& identifier : key->GetIdentifiers()) {
        std::string identifierName = identifier->GetName();
        int identifierIndex = identifier->GetIndex();

        if (!data.contains(identifierName)) return false;
        data = data[identifierName];

        if (identifierIndex >= 0) { // Identifier has an index attached to it
            if (data.size() <= identifierIndex) return false;
            data = data[identifierIndex];
        }
    }
    return true;
}

std::string getOutput(bool isSingleStatement, ExprAST* ast, const json& data) {
    if (isSingleStatement) {
        return evaluateStmtList(ast, data) ? "true" : "false";
    } else {
        return getOutputOfContent(ast, data);
    }
}

std::string getOutputOfContent(ExprAST* ast, const json& data) {
    std::string result;

    auto* content = dynamic_cast<ContentExprAST*>(ast);
    for (const std::unique_ptr<ExprAST>& section : content->GetSections()) {
        if (auto* arbitrarySection = dynamic_cast<ArbitraryExprAST*>(section.get())) { // Is section an arbitrary section?
            result += getOutputOfArbitrarySection(arbitrarySection);
        } else if (auto* sectionExpr = dynamic_cast<SectionExprAST*>(section.get())) { // Is section a relevant section?
            result += getOutputOfRelevantSection(sectionExpr, data);
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
        switch (comBlock->GetType()) {
            case ComBlockExprAST::COM_LINE_BLOCK_EXPR: {
                auto* lineBlockExpr = static_cast<ComLineBlockExprAST*>(comBlock.get());
                // Evaluate condition and append payload to output string if condition was truthy
                if (evaluateStmtList(lineBlockExpr->GetStmtList().get(), data)) {
                    result += lineBlockExpr->GetPayload()->GetValue();
                }
                break;
            }
            case ComBlockExprAST::COM_BLOCK_BLOCK_EXPR: {
                auto* blockBlockExpr = static_cast<ComBlockBlockExprAST*>(comBlock.get());
                const std::unique_ptr<IfBlockExprAST>& ifBlock = blockBlockExpr->GetIfBlock();
                // Evaluate condition and append payload to output string if condition was truthy
                if (evaluateStmtList(ifBlock->GetStmtList().get(), data)) {
                    result += ifBlock->GetPayload()->GetValue();
                }
                break;
            }
            default:
                throw std::runtime_error("Got unknown ComBlock object");
        }
    }
    return result;
}

bool evaluateStmtList(ExprAST* ast, const json& data) {
    auto* stmtList = dynamic_cast<StmtLstExprAST*>(ast);
    // Loop through statements
    for (const std::unique_ptr<StmtExprAST>& stmt : stmtList->GetStatements()) {
        switch (stmt->GetType()) {
            case StmtExprAST::HAS_STMT_EXPR: {
                auto* hasStmt = static_cast<HasStmtExprAST*>(stmt.get());
                if (evaluateHasStatement(hasStmt, data)) return true;
                continue;
            }
            case StmtExprAST::COMP_STMT_EXPR: {
                auto* compStmt = static_cast<CompStmtExprAST*>(stmt.get());
                if (evaluateCompStatement(compStmt, data)) return true;
                continue;
            }
            default:
                throw std::runtime_error("Got unknown Stmt object");
        }
    }
    return false;
}

bool evaluateHasStatement(HasStmtExprAST* ast, const json& data) {
    bool isKeyExisting = getExistenceOfJsonKey(ast->GetKey(), data);
    if (ast->GetInverted()) return !isKeyExisting;
    return isKeyExisting;
}

bool evaluateCompStatement(CompStmtExprAST* ast, const json& data) {
    json keyValue = getJsonValueFromKey(ast->GetKey(), data);
    if (keyValue.is_string()) {
        auto value = keyValue.get<std::string>();
        if (ast->GetValue()->GetType() == ValueExprAST::STRING_EXPR) {
            auto *expectedValue = static_cast<StringExprAST*>(ast->GetValue().get());
            return value == expectedValue->GetValue();
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - JSON value was string and hardcoded was not");
    } else if (keyValue.is_boolean()) {
        auto value = keyValue.get<bool>();
        if (ast->GetValue()->GetType() == ValueExprAST::BOOLEAN_EXPR) {
            auto *expectedValue = static_cast<BooleanExprAST*>(ast->GetValue().get());
            return value == expectedValue->GetValue();
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - JSON value was boolean and hardcoded was not");
    } else if (keyValue.is_number_integer()) {
        auto value = keyValue.get<int>();
        if (ast->GetValue()->GetType() == ValueExprAST::NUMBER_EXPR) {
            auto* expectedValue = static_cast<NumberExprAST*>(ast->GetValue().get());
            return value == expectedValue->GetValue();
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - JSON value was int and hardcoded was not");
    }
    throw std::runtime_error("Unknown datatype of '" + keyValue.dump() + "'");
}

std::string interpretInput(bool isSingleStatement, const std::string& fileInput,
                           const std::string& dataInput, const std::string& lineCommentChars,
                           const std::string& blockCommentCharsOpen, const std::string& blockCommentCharsClose) {
    // Parse input string from JSON to object
    json data = json::parse(dataInput);

    // Get semantically checked AST
    ExprAST* ast = executeSemanticAnalysis(isSingleStatement, fileInput, data, lineCommentChars,
                                           blockCommentCharsOpen, blockCommentCharsClose);

    return getOutput(isSingleStatement, ast, data);
}