// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#include "CompStmtExprAST.h"

Operator CompStmtExprAST::GetOperator() const {
    return Op;
}

std::unique_ptr<KeyExprAST> const &CompStmtExprAST::GetKey() {
    return Key;
}

std::unique_ptr<ValueExprAST> const &CompStmtExprAST::GetValue() {
    return Value;
}