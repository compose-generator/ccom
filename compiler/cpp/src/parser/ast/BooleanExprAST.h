//
// Created by Marc on 20.05.2021.
//

#pragma once

#include "ValueExprAST.h"

class BooleanExprAST : public ValueExprAST {
private:
    bool Value;
public:
    explicit BooleanExprAST(bool val): Value(val) {}
    bool GetValue() const;
};
