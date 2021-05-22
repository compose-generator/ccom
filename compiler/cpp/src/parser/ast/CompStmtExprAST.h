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
    OP_NOT_EQUALS
};

class CompStmtExprAST : public StmtExprAST {
private:
    Operator op;
    std::unique_ptr<KeyExprAST> key;
    std::unique_ptr<ValueExprAST> value;
public:
    explicit CompStmtExprAST(std::unique_ptr<KeyExprAST> key, Operator op, std::unique_ptr<ValueExprAST> value):
            key(std::move(key)), op(op), value(std::move(value)) {}
    Operator GetOperator() const;
    const std::unique_ptr<KeyExprAST> &GetKey();
    const std::unique_ptr<ValueExprAST> &GetValue();
};
