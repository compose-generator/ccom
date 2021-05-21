// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <string>

class PayloadExprAST {
private:
    std::string Value;
public:
    explicit PayloadExprAST(std::string val): Value(std::move(val)) {}
    std::string GetValue() const;
};
