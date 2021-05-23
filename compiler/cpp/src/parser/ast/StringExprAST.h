// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#pragma once

#include <string>
#include <utility>
#include "ValueExprAST.h"

class StringExprAST : public ValueExprAST {
private:
    std::string value;
public:
    explicit StringExprAST(std::string val): ValueExprAST(ValueExprAST::Type::STRING_EXPR), value(std::move(val)) {}
    std::string getValue() const;
};
