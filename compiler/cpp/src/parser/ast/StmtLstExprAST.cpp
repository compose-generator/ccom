// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

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