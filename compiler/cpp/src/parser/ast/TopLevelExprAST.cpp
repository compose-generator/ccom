/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "TopLevelExprAST.h"
#include "ContentExprAST.h" // This import cant be in the header file for some reason
#include "StmtLstExprAST.h" // This import cant be in the header file for some reason

TopLevelExprType TopLevelExprAST::getType() {
    return type;
}

std::string TopLevelExprAST::serialize() {
    switch(type) {
        case CONTENT_EXPR: {
            auto* contentExpr = static_cast<ContentExprAST*>(this);
            return contentExpr->serialize();
        }
        case STMT_LST_EXPR: {
            auto* stmtLstExpr = static_cast<StmtLstExprAST*>(this);
            return stmtLstExpr->serialize();
        }
        default:
            throw std::runtime_error("Unknown top level expression type");
    }
}