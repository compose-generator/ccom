// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <memory>
#include "KeyExprAST.h"
#include "ValueExprAST.h"
#include "StmtExprAST.h"

enum Operator {
    OP_EQUALS,
    OP_NOT_EQUALS,
    OP_GREATER,
    OP_LESS,
    OP_GREATER_EQUAL,
    OP_LESS_EQUAL,
};

class CompStmtExprAST : public StmtExprAST {
private:
    Operator op;
    std::unique_ptr<KeyExprAST> key;
    std::unique_ptr<ValueExprAST> value;
public:
    explicit CompStmtExprAST(std::unique_ptr<KeyExprAST> key, Operator op, std::unique_ptr<ValueExprAST> value):
            StmtExprAST(StmtExprAST::Type::COMP_STMT_EXPR), key(std::move(key)), op(op), value(std::move(value)) {}
    Operator getOperator() const;
    const std::unique_ptr<KeyExprAST> &getKey();
    const std::unique_ptr<ValueExprAST> &getValue();
};
