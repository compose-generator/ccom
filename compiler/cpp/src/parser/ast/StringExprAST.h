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
    explicit StringExprAST(std::string val): value(std::move(val)) {}
    std::string GetValue() const;
};
