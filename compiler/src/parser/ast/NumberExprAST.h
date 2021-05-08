// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#ifndef COMPILER_NUMBEREXPRAST_H
#define COMPILER_NUMBEREXPRAST_H

#include "ExprAST.h"

class NumberExprAST : public ExprAST {
private:
    int Val;
public:
    explicit NumberExprAST(int val): Val(val) {}
};

#endif //COMPILER_NUMBEREXPRAST_H
