// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 14.05.2021.
//

#include "interpreter.h"

bool getExistenceOfJsonKey(const std::unique_ptr<KeyExprAST> &key, json data) {
    for (const std::unique_ptr<IdentifierExprAST>& identifier : key->getIdentifiers()) {
        std::string identifierName = identifier->getName();
        int identifierIndex = identifier->getIndex();

        if (!data.contains(identifierName)) return false;
        data = data[identifierName];

        if (identifierIndex >= 0) { // Identifier has an index attached to it
            if (data.size() <= identifierIndex) return false;
            data = data[identifierIndex];
        }
    }
    return true;
}

std::string getOutput(bool isSingleStatement, TopLevelExprAST* ast, const json& data) {
    if (isSingleStatement) {
        if (ast->getType() != TopLevelExprAST::STMT_LST_EXPR)
            throw std::runtime_error("Input was no single statement list");
        auto* stmtLst = static_cast<StmtLstExprAST*>(ast);
        return evaluateStmtList(stmtLst, data) ? "true" : "false";
    } else {
        auto* content = static_cast<ContentExprAST*>(ast);
        return getOutputOfContent(content, data);
    }
}

std::string getOutputOfContent(ContentExprAST* content, const json& data) {
    std::string result;

    for (const std::unique_ptr<ContentBlockExprAST>& contentBlock : content->getContentBlocks()) {
        if (contentBlock->getType() == ContentBlockExprAST::ARBITRARY_EXPR) { // Is section an arbitrary section?
            auto* arbitrarySection = static_cast<ArbitraryExprAST*>(contentBlock.get());
            result += getOutputOfArbitrarySection(arbitrarySection);
        } else if (contentBlock->getType() == ContentBlockExprAST::SECTION_EXPR) { // Is section a relevant section?
            auto* sectionExpr = static_cast<SectionExprAST*>(contentBlock.get());
            result += getOutputOfRelevantSection(sectionExpr, data);
        }
    }
    return result;
}

std::string getOutputOfArbitrarySection(ArbitraryExprAST* arbitraryExpr) {
    return arbitraryExpr->getValue();
}

std::string getOutputOfRelevantSection(SectionExprAST* relevantSection, const json& data) {
    std::string result;

    // Loop through com blocks
    for (const std::unique_ptr<ComBlockExprAST>& comBlock : relevantSection->getComBlocks()) {
        switch (comBlock->getType()) {
            case ComBlockExprAST::COM_LINE_BLOCK_EXPR: {
                auto* lineBlockExpr = static_cast<ComLineBlockExprAST*>(comBlock.get());
                // Evaluate condition and append payload to output string if condition was truthy
                if (evaluateStmtList(lineBlockExpr->getStmtList().get(), data)) {
                    result += lineBlockExpr->getPayload()->getValue();
                }
                break;
            }
            case ComBlockExprAST::COM_BLOCK_BLOCK_EXPR: {
                auto* blockBlockExpr = static_cast<ComBlockBlockExprAST*>(comBlock.get());
                const std::unique_ptr<IfBlockExprAST>& ifBlock = blockBlockExpr->getIfBlock();
                // Evaluate condition and append payload to output string if condition was truthy
                if (evaluateStmtList(ifBlock->getStmtList().get(), data)) {
                    result += ifBlock->getPayload()->getValue();
                }
                break;
            }
            default:
                throw std::runtime_error("Got unknown ComBlock object");
        }
    }
    return result;
}

bool evaluateStmtList(StmtLstExprAST* stmtList, const json& data) {
    // Loop through statements
    for (const std::unique_ptr<StmtExprAST>& stmt : stmtList->getStatements()) {
        switch (stmt->getType()) {
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

bool evaluateHasStatement(HasStmtExprAST* hasStmt, const json& data) {
    bool isKeyExisting = getExistenceOfJsonKey(hasStmt->getKey(), data);
    if (hasStmt->getInverted()) return !isKeyExisting;
    return isKeyExisting;
}

bool evaluateCompStatement(CompStmtExprAST* compStmt, const json& data) {
    json keyValue = getJsonValueFromKey(compStmt->getKey(), data);
    Operator op = compStmt->getOperator();
    if (keyValue.is_string()) {
        auto leftValue = keyValue.get<std::string>();
        if (compStmt->getValue()->getType() == ValueExprAST::STRING_EXPR) {
            auto *rightValue = static_cast<StringExprAST*>(compStmt->getValue().get());
            return evaluateCondition(leftValue, rightValue->getValue(), op);
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - left value was string and right was not");
    } else if (keyValue.is_boolean()) {
        auto leftValue = keyValue.get<bool>();
        if (compStmt->getValue()->getType() == ValueExprAST::BOOLEAN_EXPR) {
            auto *rightValue = static_cast<BooleanExprAST*>(compStmt->getValue().get());
            return evaluateCondition(leftValue, rightValue->getValue(), op);
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - left value was boolean and right was not");
    } else if (keyValue.is_number_integer()) {
        auto leftValue = keyValue.get<int>();
        if (compStmt->getValue()->getType() == ValueExprAST::NUMBER_EXPR) {
            auto* rightValue = static_cast<NumberExprAST*>(compStmt->getValue().get());
            return evaluateCondition(leftValue, rightValue->getValue(), op);
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - left value was int and right was not");
    }
    throw std::runtime_error("Unknown datatype of '" + keyValue.dump() + "'");
}

template <typename T> bool evaluateCondition(T leftValue, T rightValue, Operator op) {
    switch (op) {
        case OP_EQUALS: return leftValue == rightValue;
        case OP_NOT_EQUALS: return leftValue != rightValue;
        case OP_GREATER: return leftValue > rightValue;
        case OP_LESS: return leftValue < rightValue;
        case OP_GREATER_EQUAL: return leftValue >= rightValue;
        case OP_LESS_EQUAL: return leftValue <= rightValue;
    }
}

std::string interpretInput(bool isSingleStatement, const std::string& fileInput,
                           const std::string& dataInput, const std::string& lineCommentChars,
                           const std::string& blockCommentCharsOpen, const std::string& blockCommentCharsClose) {
    // Parse input string from JSON to object
    json data = json::parse(dataInput);

    // Get semantically checked AST
    TopLevelExprAST* ast = executeSemanticAnalysis(isSingleStatement, fileInput, data, lineCommentChars,
                                           blockCommentCharsOpen, blockCommentCharsClose);

    return getOutput(isSingleStatement, ast, data);
}