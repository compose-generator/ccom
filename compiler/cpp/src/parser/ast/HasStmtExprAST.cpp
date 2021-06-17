// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

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