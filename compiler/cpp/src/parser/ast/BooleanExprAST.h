/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

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
