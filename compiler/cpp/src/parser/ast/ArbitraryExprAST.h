// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

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
