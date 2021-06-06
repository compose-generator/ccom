//
// Created by Marc on 20.05.2021.
//

#pragma once

#include "ValueExprAST.h"

class BooleanExprAST : public ValueExprAST {
public:
    // Constructors
    explicit BooleanExprAST(bool val): ValueExprAST(ValueExprType::BOOLEAN_EXPR), value(val) {}

    // Public methods
    bool getValue() const;
private:
    // Members
    bool value;
};
