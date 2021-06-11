// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

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
protected:
    // Protected constructors
    explicit ValueExprAST(ValueExprType t): type(t) {}
private:
    // Members
    ValueExprType type;
};
