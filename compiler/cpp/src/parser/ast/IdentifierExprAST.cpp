// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#include "IdentifierExprAST.h"

std::string IdentifierExprAST::getName() const {
    return name;
}

int IdentifierExprAST::getIndex() const {
    return index;
}

std::string IdentifierExprAST::serialize() const {
    return "identifier(name: '" + name + "', index: '" + std::to_string(index) + "')";
}