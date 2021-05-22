// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <vector>
#include <memory>
#include "IdentifierExprAST.h"

class KeyExprAST {
private:
    std::vector<std::unique_ptr<IdentifierExprAST>> identifiers;
public:
    explicit KeyExprAST(std::vector<std::unique_ptr<IdentifierExprAST>> stmts): identifiers(std::move(stmts)) {}
    const std::vector<std::unique_ptr<IdentifierExprAST>> &GetIdentifiers();
};