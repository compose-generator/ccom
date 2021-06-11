// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

#include <string>

class PayloadExprAST {
public:
    // Constructors
    explicit PayloadExprAST(std::string val): value(std::move(val)) {}

    // Public methods
    std::string getValue() const;
private:
    // Members
    std::string value;
};
