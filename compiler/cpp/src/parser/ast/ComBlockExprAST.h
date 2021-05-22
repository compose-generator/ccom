// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include "ExprAST.h"

class ComBlockExprAST : public ExprAST {
public:
    enum Type { COM_BLOCK_EXPR, COM_LINE_BLOCK_EXPR, COM_BLOCK_BLOCK_EXPR };

    explicit ComBlockExprAST(): type(COM_BLOCK_EXPR) {}
    Type GetType();
protected:
    explicit ComBlockExprAST(Type t): type(t) {}
private:
    Type type;
};
