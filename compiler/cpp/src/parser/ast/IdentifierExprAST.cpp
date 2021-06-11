// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "IdentifierExprAST.h"

std::string IdentifierExprAST::getName() const {
    return name;
}

int IdentifierExprAST::getIndex() const {
    return index;
}