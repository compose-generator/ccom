// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#include "NumberExprAST.h"

int NumberExprAST::getValue() const {
    return value;
}

std::string NumberExprAST::serialize() const {
    return "number(value: '" + std::to_string(value) + "')";
}
