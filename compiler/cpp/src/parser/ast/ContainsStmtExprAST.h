// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

#include <memory>
#include "KeyExprAST.h"
#include "CompStmtExprAST.h"

class ContainsStmtExprAST : public StmtExprAST {
public:
    // Constructors
    explicit ContainsStmtExprAST(std::unique_ptr<KeyExprAST> listKey, std::unique_ptr<KeyExprAST> valueKey,
                                 std::unique_ptr<ValueExprAST> value, Operator op, bool inverted):
        StmtExprAST(StmtExprType::CONTAINS_STMT_EXPR), listKey(std::move(listKey)), valueKey(std::move(valueKey)),
        value(std::move(value)), op(op), isInverted(inverted) {}

    // Public methods
    Operator getOperator() const;
    const std::unique_ptr<KeyExprAST> &getListKey();
    const std::unique_ptr<KeyExprAST> &getValueKey();
    const std::unique_ptr<ValueExprAST> &getValue();
    bool getInverted() const;
    std::string serialize() const;
private:
    // Members
    Operator op;
    std::unique_ptr<KeyExprAST> listKey;
    std::unique_ptr<KeyExprAST> valueKey;
    std::unique_ptr<ValueExprAST> value;
    bool isInverted;
};