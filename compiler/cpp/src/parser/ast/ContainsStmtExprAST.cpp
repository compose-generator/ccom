// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "ContainsStmtExprAST.h"

Operator ContainsStmtExprAST::getOperator() const {
    return op;
}

std::unique_ptr<KeyExprAST> const &ContainsStmtExprAST::getListKey() {
    return listKey;
}

std::unique_ptr<KeyExprAST> const &ContainsStmtExprAST::getValueKey() {
    return valueKey;
}

std::unique_ptr<ValueExprAST> const &ContainsStmtExprAST::getValue() {
    return value;
}