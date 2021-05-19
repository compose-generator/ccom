// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#ifndef COMPILER_IDENTIFIEREXPRAST_H
#define COMPILER_IDENTIFIEREXPRAST_H

#include <string>
#include <utility>
#include "ExprAST.h"

class IdentifierExprAST: public ExprAST {
private:
    std::string Name;
    int Index = -1;
public:
    explicit IdentifierExprAST(std::string Name): Name(std::move(Name)) {}
    IdentifierExprAST(std::string name, int index): Name(std::move(name)), Index(index) {}
    std::string GetName() const;
    int GetIndex() const;
};

#endif //COMPILER_IDENTIFIEREXPRAST_H
