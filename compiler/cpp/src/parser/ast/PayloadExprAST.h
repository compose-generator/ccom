// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <string>

class PayloadExprAST {
public:
    // Constructors
    explicit PayloadExprAST(std::string val): value(std::move(val)) {}

    // Public methods
    std::string getValue() const;
    std::string serialize() const;
private:
    // Members
    std::string value;
};
