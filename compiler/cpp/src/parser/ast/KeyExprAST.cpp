// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "KeyExprAST.h"

std::vector<std::unique_ptr<IdentifierExprAST>> const &KeyExprAST::getIdentifiers() {
    return identifiers;
}