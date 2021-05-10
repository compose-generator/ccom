// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#ifndef COMPILER_STRINGEXPRAST_H
#define COMPILER_STRINGEXPRAST_H

#include <string>
#include <utility>
#include "ValueExprAST.h"

class StringExprAST : public ValueExprAST {
private:
    std::string Value;
public:
    explicit StringExprAST(std::string val): Value(std::move(val)) {}
    std::string GetValue() const;
};

#endif //COMPILER_STRINGEXPRAST_H
