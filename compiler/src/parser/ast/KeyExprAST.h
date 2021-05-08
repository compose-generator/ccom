// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_KEYEXPRAST_H
#define COMPILER_KEYEXPRAST_H

#include <vector>
#include <memory>
#include "IdentifierExprAST.h"

class KeyExprAST {
private:
    std::vector<std::unique_ptr<IdentifierExprAST>> Identifiers;
public:
    explicit KeyExprAST(std::vector<std::unique_ptr<IdentifierExprAST>> stmts): Identifiers(std::move(stmts)) {}
};

#endif //COMPILER_KEYEXPRAST_H
