// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#pragma once

#include <string>
#include <utility>
#include "ValueExprAST.h"

class StringExprAST : public ValueExprAST {
public:
    // Constructors
    explicit StringExprAST(std::string val): ValueExprAST(ValueExprType::STRING_EXPR), value(std::move(val)) {}

    // Public methods
    std::string getValue() const;
    std::string serialize() const;
private:
    // Members
    std::string value;
};
