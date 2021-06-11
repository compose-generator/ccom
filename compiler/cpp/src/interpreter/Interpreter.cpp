// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "Interpreter.h"

Interpreter::Interpreter(bool isSingleStatement, const std::string &fileInput, const std::string &dataInput,
                         const std::string &inputCommentLineIdentifiers,
                         const std::string &inputCommentBlockOpenIdentifiers,
                         const std::string &inputCommentBlockCloseIdentifiers) {
    this->isSingleStatement = isSingleStatement;

    // Parse input string from JSON to object
    jsonParser = JSONParser(dataInput);

    // Initialize analyzer
    analyzer = Analyzer(isSingleStatement, fileInput, jsonParser, inputCommentLineIdentifiers,
                        inputCommentBlockOpenIdentifiers, inputCommentBlockCloseIdentifiers);
    ast = analyzer.getAST();

    // Execute semantic analysis
    analyzer.executeAnalysis();
}

std::string Interpreter::interpretInput() {
    return getOutput();
}

std::string Interpreter::getOutput() {
    if (isSingleStatement) {
        if (ast->getType() != TopLevelExprType::STMT_LST_EXPR)
            throw std::runtime_error("Input was no single statement list");
        auto* stmtLst = static_cast<StmtLstExprAST*>(ast);
        return evaluateStmtList(stmtLst) ? "true" : "false";
    } else {
        auto* content = static_cast<ContentExprAST*>(ast);
        return getOutputOfContent(content);
    }
}

std::string Interpreter::getOutputOfContent(ContentExprAST* content) {
    std::string result;

    for (const std::unique_ptr<ContentBlockExprAST>& contentBlock : content->getContentBlocks()) {
        if (contentBlock->getType() == ContentBlockExprType::ARBITRARY_EXPR) { // Is section an arbitrary section?
            auto* arbitrarySection = static_cast<ArbitraryExprAST*>(contentBlock.get());
            result += getOutputOfArbitrarySection(arbitrarySection);
        } else if (contentBlock->getType() == ContentBlockExprType::SECTION_EXPR) { // Is section a relevant section?
            auto* sectionExpr = static_cast<SectionExprAST*>(contentBlock.get());
            result += getOutputOfRelevantSection(sectionExpr);
        }
    }
    return result;
}

std::string Interpreter::getOutputOfArbitrarySection(ArbitraryExprAST* arbitraryExpr) {
    return arbitraryExpr->getValue();
}

std::string Interpreter::getOutputOfRelevantSection(SectionExprAST* relevantSection) {
    std::string result;

    // Loop through com blocks
    for (const std::unique_ptr<ComBlockExprAST>& comBlock : relevantSection->getComBlocks()) {
        switch (comBlock->getType()) {
            case ComBlockExprType::COM_LINE_BLOCK_EXPR: {
                auto* lineBlockExpr = static_cast<ComLineBlockExprAST*>(comBlock.get());
                // Evaluate condition and append payload to output string if condition was truthy
                if (evaluateStmtList(lineBlockExpr->getStmtList().get())) {
                    result += lineBlockExpr->getPayload()->getValue();
                }
                break;
            }
            case ComBlockExprType::COM_BLOCK_BLOCK_EXPR: {
                auto* blockBlockExpr = static_cast<ComBlockBlockExprAST*>(comBlock.get());
                const std::unique_ptr<IfBlockExprAST>& ifBlock = blockBlockExpr->getIfBlock();
                // Evaluate condition and append payload to output string if condition was truthy
                if (evaluateStmtList(ifBlock->getStmtList().get())) {
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

bool Interpreter::evaluateStmtList(StmtLstExprAST* stmtList) {
    // Loop through statements
    for (const std::unique_ptr<StmtExprAST>& stmt : stmtList->getStatements()) {
        switch (stmt->getType()) {
            case StmtExprType::HAS_STMT_EXPR: {
                auto* hasStmt = static_cast<HasStmtExprAST*>(stmt.get());
                if (evaluateHasStatement(hasStmt)) return true;
                continue;
            }
            case StmtExprType::COMP_STMT_EXPR: {
                auto* compStmt = static_cast<CompStmtExprAST*>(stmt.get());
                if (evaluateCompStatement(compStmt)) return true;
                continue;
            }
            default:
                throw std::runtime_error("Got unknown Stmt object");
        }
    }
    return false;
}

bool Interpreter::evaluateHasStatement(HasStmtExprAST* hasStmt) {
    bool isKeyExisting = jsonParser.jsonKeyExists(hasStmt->getKey());
    if (hasStmt->getInverted()) return !isKeyExisting;
    return isKeyExisting;
}

bool Interpreter::evaluateCompStatement(CompStmtExprAST* compStmt) {
    json keyValue = jsonParser.getJSONValueFromKey(compStmt->getKey());
    Operator op = compStmt->getOperator();
    if (keyValue.is_string()) {
        auto leftValue = keyValue.get<std::string>();
        if (compStmt->getValue()->getType() == ValueExprType::STRING_EXPR) {
            auto *rightValue = static_cast<StringExprAST*>(compStmt->getValue().get());
            return evaluateCondition(leftValue, rightValue->getValue(), op);
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - left value was string and right was not");
    } else if (keyValue.is_boolean()) {
        auto leftValue = keyValue.get<bool>();
        if (compStmt->getValue()->getType() == ValueExprType::BOOLEAN_EXPR) {
            auto *rightValue = static_cast<BooleanExprAST*>(compStmt->getValue().get());
            return evaluateCondition(leftValue, rightValue->getValue(), op);
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - left value was boolean and right was not");
    } else if (keyValue.is_number_integer()) {
        auto leftValue = keyValue.get<int>();
        if (compStmt->getValue()->getType() == ValueExprType::NUMBER_EXPR) {
            auto* rightValue = static_cast<NumberExprAST*>(compStmt->getValue().get());
            return evaluateCondition(leftValue, rightValue->getValue(), op);
        }
        // This should never get triggered, because invalid type combinations are already filtered out
        throw std::runtime_error("Internal compiler error - left value was int and right was not");
    }
    throw std::runtime_error("Unknown datatype of '" + keyValue.dump() + "'");
}

template<typename T> bool Interpreter::evaluateCondition(T leftValue, T rightValue, Operator op) {
    switch (op) {
        case OP_EQUALS: return leftValue == rightValue;
        case OP_NOT_EQUALS: return leftValue != rightValue;
        case OP_GREATER: return leftValue > rightValue;
        case OP_LESS: return leftValue < rightValue;
        case OP_GREATER_EQUAL: return leftValue >= rightValue;
        case OP_LESS_EQUAL: return leftValue <= rightValue;
    }
    return false;
}
