// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_COMBLOCKEXPRAST_H
#define COMPILER_COMBLOCKEXPRAST_H

#include "ExprAST.h"

class ComBlockExprAST : public ExprAST {
public:
    virtual ~ComBlockExprAST() = default;
};

#endif //COMPILER_COMBLOCKEXPRAST_H
