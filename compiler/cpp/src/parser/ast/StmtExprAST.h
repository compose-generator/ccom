// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <vector>
#include "ExprAST.h"

class StmtExprAST: public ExprAST {
public:
    enum Type { STMT_EXPR, HAS_STMT_EXPR, COMP_STMT_EXPR };

    explicit StmtExprAST(): type(STMT_EXPR) {}
    Type GetType();
protected:
    explicit StmtExprAST(Type t): type(t) {}
private:
    Type type;
};
