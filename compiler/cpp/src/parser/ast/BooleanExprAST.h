//
// Created by Marc on 20.05.2021.
//

#pragma once

#include "ValueExprAST.h"

class BooleanExprAST : public ValueExprAST {
private:
    bool value;
public:
    explicit BooleanExprAST(bool val): ValueExprAST(ValueExprAST::Type::BOOLEAN_EXPR), value(val) {}
    bool getValue() const;
};
