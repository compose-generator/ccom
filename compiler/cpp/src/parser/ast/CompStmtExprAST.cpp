// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

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