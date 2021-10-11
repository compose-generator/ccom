/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "HasStmtExprAST.h"

std::unique_ptr<KeyExprAST> const &HasStmtExprAST::getKey() {
    return key;
}

bool HasStmtExprAST::getInverted() const {
    return isInverted;
};

std::string HasStmtExprAST::serialize() const {
    return "has-stmt(key: " + key->serialize() + ", isInverted: " + std::to_string(isInverted) + ")";
}