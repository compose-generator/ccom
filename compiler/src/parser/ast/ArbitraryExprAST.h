// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_ARBITRARYEXPRAST_H
#define COMPILER_ARBITRARYEXPRAST_H

#include <string>
#include <utility>
#include "ExprAST.h"

class ArbitraryExprAST : public ExprAST {
private:
    std::string Value;
public:
    explicit ArbitraryExprAST(std::string value): Value(std::move(value)) {}
    std::string GetValue() const;
};

#endif //COMPILER_ARBITRARYEXPRAST_H
