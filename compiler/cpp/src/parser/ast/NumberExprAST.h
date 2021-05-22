// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#pragma once

#include "ValueExprAST.h"

class NumberExprAST : public ValueExprAST {
private:
    int value;
public:
    explicit NumberExprAST(int val): ValueExprAST(ValueExprAST::Type::NUMBER_EXPR), value(val) {}
    int GetValue() const;
};
