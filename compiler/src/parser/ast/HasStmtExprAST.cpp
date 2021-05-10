// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#include "HasStmtExprAST.h"

std::unique_ptr<KeyExprAST> const &HasStmtExprAST::GetKey() {
    return Key;
}

bool HasStmtExprAST::GetInverted() const {
    return Inverted;
};