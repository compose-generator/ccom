// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_STMTEXPRAST_H
#define COMPILER_STMTEXPRAST_H

#include <vector>
#include "ExprAST.h"

class StmtExprAST: public ExprAST {
public:
    ~StmtExprAST() override = default;
};

#endif //COMPILER_STMTEXPRAST_H
