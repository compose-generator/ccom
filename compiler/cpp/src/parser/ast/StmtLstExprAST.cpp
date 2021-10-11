/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "StmtLstExprAST.h"

std::vector<std::unique_ptr<StmtExprAST>> const &StmtLstExprAST::getStatements() {
    return stmts;
}

std::string StmtLstExprAST::serialize() const {
    std::string serializedStatements;
    for (auto& stmt : stmts) {
        serializedStatements += stmt->serialize();
    }
    return "stmt-lst(" + serializedStatements + ")";
}