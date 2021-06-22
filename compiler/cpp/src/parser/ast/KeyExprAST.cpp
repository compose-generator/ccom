// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "KeyExprAST.h"

std::vector<std::unique_ptr<IdentifierExprAST>> const &KeyExprAST::getIdentifiers() {
    return identifiers;
}

std::string KeyExprAST::serialize() const {
    std::string result = "key(";
    for (auto& identifier : identifiers) {
        result += identifier->serialize();
    }
    return result + ")";
}