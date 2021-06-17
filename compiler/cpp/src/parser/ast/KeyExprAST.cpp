// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

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