// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <vector>

class StmtExprAST {
public:
    enum Type { STMT_EXPR, HAS_STMT_EXPR, COMP_STMT_EXPR };

    explicit StmtExprAST(): type(STMT_EXPR) {}
    Type getType();
protected:
    explicit StmtExprAST(Type t): type(t) {}
private:
    Type type;
};
