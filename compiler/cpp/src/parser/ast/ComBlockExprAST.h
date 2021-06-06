// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

enum ComBlockExprType {
    COM_BLOCK_EXPR,
    COM_LINE_BLOCK_EXPR,
    COM_BLOCK_BLOCK_EXPR
};

class ComBlockExprAST {
public:
    // Constructors
    explicit ComBlockExprAST(): type(COM_BLOCK_EXPR) {}

    // Public methods
    ComBlockExprType getType();
protected:
    // Protected constructors
    explicit ComBlockExprAST(ComBlockExprType t): type(t) {}
private:
    // Members
    ComBlockExprType type;
};