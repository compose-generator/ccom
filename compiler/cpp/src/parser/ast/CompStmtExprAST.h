/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <memory>
#include "KeyExprAST.h"
#include "ValueExprAST.h"
#include "StmtExprAST.h"
#include "NumberExprAST.h"
#include "BooleanExprAST.h"
#include "StringExprAST.h"

enum Operator {
    OP_EQUALS,
    OP_NOT_EQUALS,
    OP_GREATER,
    OP_LESS,
    OP_GREATER_EQUAL,
    OP_LESS_EQUAL,
};

class CompStmtExprAST : public StmtExprAST {
public:
    // Constructors
    explicit CompStmtExprAST(std::unique_ptr<KeyExprAST> key, Operator op, std::unique_ptr<ValueExprAST> value):
            StmtExprAST(StmtExprType::COMP_STMT_EXPR), key(std::move(key)), op(op), value(std::move(value)) {}

    // Public methods
    Operator getOperator() const;
    const std::unique_ptr<KeyExprAST> &getKey();
    const std::unique_ptr<ValueExprAST> &getValue();
    std::string serialize() const;
private:
    // Members
    Operator op;
    std::unique_ptr<KeyExprAST> key;
    std::unique_ptr<ValueExprAST> value;
};
