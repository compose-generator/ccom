// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "HasStmtExprAST.h"

std::unique_ptr<KeyExprAST> const &HasStmtExprAST::getKey() {
    return key;
}

bool HasStmtExprAST::getInverted() const {
    return isInverted;
};