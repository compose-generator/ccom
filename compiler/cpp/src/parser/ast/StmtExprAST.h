// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <vector>

enum StmtExprType {
    STMT_EXPR,
    HAS_STMT_EXPR,
    COMP_STMT_EXPR
};

class StmtExprAST {
public:
    // Constructors
    explicit StmtExprAST(): type(STMT_EXPR) {}

    // Public methods
    StmtExprType getType();
protected:
    // Protected constructors
    explicit StmtExprAST(StmtExprType t): type(t) {}
private:
    // Members
    StmtExprType type;
};
