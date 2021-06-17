// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

#include "ValueExprAST.h"
#include <string>

class NumberExprAST : public ValueExprAST {
public:
    // Constructors
    explicit NumberExprAST(int val): ValueExprAST(ValueExprType::NUMBER_EXPR), value(val) {}

    // Public methods
    int getValue() const;
    std::string serialize() const;
private:
    // Members
    int value;
};
