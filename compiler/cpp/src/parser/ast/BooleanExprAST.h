#pragma once

#include "ValueExprAST.h"
#include <string>

class BooleanExprAST : public ValueExprAST {
public:
    // Constructors
    explicit BooleanExprAST(bool val): ValueExprAST(ValueExprType::BOOLEAN_EXPR), value(val) {}

    // Public methods
    bool getValue() const;
    std::string serialize() const;
private:
    // Members
    bool value;
};
