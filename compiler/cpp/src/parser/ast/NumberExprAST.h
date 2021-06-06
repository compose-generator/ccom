// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#pragma once

#include "ValueExprAST.h"

class NumberExprAST : public ValueExprAST {
public:
    // Constructors
    explicit NumberExprAST(int val): ValueExprAST(ValueExprType::NUMBER_EXPR), value(val) {}

    // Public methods
    int getValue() const;
private:
    // Members
    int value;
};
