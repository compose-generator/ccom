// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <string>
#include <utility>
#include "ContentBlockExprAST.h"

class ArbitraryExprAST : public ContentBlockExprAST {
private:
    std::string value;
public:
    explicit ArbitraryExprAST(std::string value): ContentBlockExprAST(ContentBlockExprAST::Type::ARBITRARY_EXPR), value(std::move(value)) {}
    std::string getValue() const;
};
