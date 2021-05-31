// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <string>
#include <utility>
#include "ContentBlockExprAST.h"

class ArbitraryExprAST : public ContentBlockExprAST {
public:
    // Constructors
    explicit ArbitraryExprAST(std::string value): ContentBlockExprAST(ContentBlockExprType::ARBITRARY_EXPR),
        value(std::move(value)) {}

    // Public methods
    std::string getValue() const;

private:
    // Members
    std::string value;
};
