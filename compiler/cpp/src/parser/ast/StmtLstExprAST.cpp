// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "StmtLstExprAST.h"

std::vector<std::unique_ptr<StmtExprAST>> const &StmtLstExprAST::getStatements() {
    return stmts;
}

std::string StmtLstExprAST::serialize() const {
    std::string result = "stmt-lst(";
    for (auto& stmt : stmts) {
        switch (stmt->getType()) {
            case StmtExprType::HAS_STMT_EXPR: {
                auto* hasStmt = static_cast<HasStmtExprAST*>(stmt.get());
                result += hasStmt->serialize();
                continue;
            }
            case StmtExprType::COMP_STMT_EXPR: {
                auto* compStmt = static_cast<CompStmtExprAST*>(stmt.get());
                result += compStmt->serialize();
                continue;
            }
            default:
                throw std::runtime_error("Got unknown Stmt object");
        }
    }
    return result + ")";
}