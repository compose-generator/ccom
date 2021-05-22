// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#include "CompStmtExprAST.h"

Operator CompStmtExprAST::getOperator() const {
    return op;
}

std::unique_ptr<KeyExprAST> const &CompStmtExprAST::getKey() {
    return key;
}

std::unique_ptr<ValueExprAST> const &CompStmtExprAST::getValue() {
    return value;
}