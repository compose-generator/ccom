// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#ifndef COMPILER_NUMBEREXPRAST_H
#define COMPILER_NUMBEREXPRAST_H

#include "ValueExprAST.h"

class NumberExprAST : public ValueExprAST {
private:
    int Value;
public:
    explicit NumberExprAST(int val): Value(val) {}
    int GetValue() const;
};

#endif //COMPILER_NUMBEREXPRAST_H
