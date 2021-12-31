/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#include "Analyzer.h"

// -------------------------------------------------- Public functions -------------------------------------------------

Analyzer::Analyzer(bool isSingleStatement, const std::string &fileInput, JSONParser jsonParser,
                   const std::string &inputCommentLineIdentifiers, const std::string &inputCommentBlockOpenIdentifiers,
                   const std::string &inputCommentBlockCloseIdentifiers): jsonParser(std::move(jsonParser)) {
    this->isSingleStatement = isSingleStatement;

    // Initialize parser
    parser = Parser(isSingleStatement, fileInput, inputCommentLineIdentifiers,
                    inputCommentBlockOpenIdentifiers, inputCommentBlockCloseIdentifiers);

    // Parse abstract syntax tree
    ast = parser.parseAST();
}

TopLevelExprAST *Analyzer::getAST() {
    return ast;
}

void Analyzer::executeAnalysis() {
    // Execute checks
    checkDataTypeCompatibility();
    // ToDo: Add more checks here
}

// ------------------------------------------------- Private functions -------------------------------------------------

void Analyzer::checkDataTypeCompatibility() {
    if (isSingleStatement) {
        if (ast->getType() != TopLevelExprType::STMT_LST_EXPR)
            throw std::runtime_error("Input was no single statement list");
        auto* stmtLst = static_cast<StmtLstExprAST*>(ast);
        checkDataTypeCompatibilityStmtList(stmtLst);
    } else {
        auto* content = static_cast<ContentExprAST*>(ast);
        checkDataTypeCompatibilityContent(content);
    }
}

void Analyzer::checkDataTypeCompatibilityContent(ContentExprAST* content) {
    // Loop through sections
    for (const std::unique_ptr<ContentBlockExprAST>& contentBlock : content->getContentBlocks()) {
        if (contentBlock->getType() == ContentBlockExprType::SECTION_EXPR) {
            auto* relevantSection = static_cast<SectionExprAST*>(contentBlock.get());
            // Loop through comBlocks
            for (const std::unique_ptr<ComBlockExprAST>& comBlock : relevantSection->getComBlocks()) {
                switch (comBlock->getType()) {
                    case ComBlockExprType::COM_LINE_BLOCK_EXPR: {
                        auto* lineBlockExpr = static_cast<ComLineBlockExprAST*>(comBlock.get());
                        checkDataTypeCompatibilityStmtList(lineBlockExpr->getIfBlock()->getStmtList().get());
                        continue;
                    }
                    case ComBlockExprType::COM_BLOCK_BLOCK_EXPR: {
                        auto* blockBlockExpr = static_cast<ComBlockBlockExprAST*>(comBlock.get());
                        checkDataTypeCompatibilityStmtList(blockBlockExpr->getIfBlock()->getStmtList().get());
                        continue;
                    }
                    default:
                        throw std::runtime_error("Got unknown ComBlock object");
                }
            }
        }
    }
}

void Analyzer::checkDataTypeCompatibilityStmtList(StmtLstExprAST* stmtLst) {
    // Loop through statements
    for (const std::unique_ptr<StmtExprAST>& stmt : stmtLst->getStatements()) {
        if (stmt->getType() == StmtExprType::COMP_STMT_EXPR) {
            auto* compStmt = static_cast<CompStmtExprAST*>(stmt.get());
            checkDataTypeCompatibilityCompStmt(compStmt);
        }
    }
}

void Analyzer::checkDataTypeCompatibilityCompStmt(CompStmtExprAST* compStmt) {
    // Abort check successfully when the key does not exist. Non-existent keys will be evaluated to false by the interpreter
    if (!jsonParser.jsonKeyExists(compStmt->getKey())) return;
    // Check if 'value' has the same type as the JSON key value
    auto jsonKeyValue = jsonParser.getJSONValueFromKey(compStmt->getKey());

    switch (compStmt->getValue()->getType()) {
        case ValueExprType::STRING_EXPR:
            if (!jsonKeyValue.is_string())
                throw IncompatibleTypesException(jsonKeyValue.dump(), "a string");
            return;
        case ValueExprType::BOOLEAN_EXPR:
            if (!jsonKeyValue.is_boolean())
                throw IncompatibleTypesException(jsonKeyValue.dump(), "a boolean");
            return;
        case ValueExprType::NUMBER_EXPR:
            if (!jsonKeyValue.is_number_integer())
                throw IncompatibleTypesException(jsonKeyValue.dump(), "an integer");
            return;
        case ValueExprType::VALUE_EXPR:
            throw UnknownDataTypeException(jsonKeyValue.dump());
    }
}