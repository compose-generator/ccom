// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <vector>
#include <memory>
#include "IdentifierExprAST.h"

class KeyExprAST {
public:
    // Constructors
    explicit KeyExprAST(std::vector<std::unique_ptr<IdentifierExprAST>> stmts): identifiers(std::move(stmts)) {}

    // Public methods
    const std::vector<std::unique_ptr<IdentifierExprAST>> &getIdentifiers();
    std::string serialize() const;
private:
    // Members
    std::vector<std::unique_ptr<IdentifierExprAST>> identifiers;
};