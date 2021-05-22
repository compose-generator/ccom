//
// Created by Marc on 20.05.2021.
//

#pragma once

#include "ValueExprAST.h"

class BooleanExprAST : public ValueExprAST {
private:
    bool value;
public:
    explicit BooleanExprAST(bool val): value(val) {}
    bool GetValue() const;
};
