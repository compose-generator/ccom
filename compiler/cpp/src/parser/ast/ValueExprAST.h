/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>
#include <stdexcept>

enum ValueExprType {
    VALUE_EXPR,
    NUMBER_EXPR,
    BOOLEAN_EXPR,
    STRING_EXPR
};

class ValueExprAST {
public:
    // Constructors
    explicit ValueExprAST(): type(VALUE_EXPR) {}

    // Public methods
    ValueExprType getType();
    std::string serialize();
protected:
    // Protected constructors
    explicit ValueExprAST(ValueExprType t): type(t) {}
private:
    // Members
    ValueExprType type;
};
