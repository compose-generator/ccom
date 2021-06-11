// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

#include <memory>
#include "KeyExprAST.h"
#include "CompStmtExprAST.h"

class ContainsStmtExprAST {
public:
    // Constructors

    // Public methods
    Operator getOperator() const;
    const std::unique_ptr<KeyExprAST> &getListKey();
    const std::unique_ptr<KeyExprAST> &getValueKey();
    const std::unique_ptr<ValueExprAST> &getValue();
private:
    // Members
    Operator op;
    std::unique_ptr<KeyExprAST> listKey;
    std::unique_ptr<KeyExprAST> valueKey;
    std::unique_ptr<ValueExprAST> value;

};