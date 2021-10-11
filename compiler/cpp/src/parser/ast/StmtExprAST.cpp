/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include <stdexcept>
#include "StmtExprAST.h"
#include "HasStmtExprAST.h" // This import cant be in the header file for some reason
#include "CompStmtExprAST.h" // This import cant be in the header file for some reason
#include "ContainsStmtExprAST.h" // This import cant be in the header file for some reason

StmtExprType StmtExprAST::getType() {
    return type;
}

std::string StmtExprAST::serialize() {
    switch(type) {
        case HAS_STMT_EXPR: {
            auto* hasStmt = static_cast<HasStmtExprAST*>(this);
            return hasStmt->serialize();
        }
        case COMP_STMT_EXPR: {
            auto* compStmt = static_cast<CompStmtExprAST*>(this);
            return compStmt->serialize();
        }
        case CONTAINS_STMT_EXPR: {
            auto* containsStmt = static_cast<ContainsStmtExprAST*>(this);
            return containsStmt->serialize();
        }
        default:
            throw std::runtime_error("Unknown value type");
    }
}