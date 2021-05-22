// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <memory>
#include "ExprAST.h"
#include "StmtLstExprAST.h"
#include "PayloadExprAST.h"
#include "ComBlockExprAST.h"

class ComLineBlockExprAST : public ComBlockExprAST {
private:
    std::unique_ptr<StmtLstExprAST> stmtList;
    std::unique_ptr<PayloadExprAST> payload;
public:
    explicit ComLineBlockExprAST(std::unique_ptr<StmtLstExprAST> stmtList, std::unique_ptr<PayloadExprAST> payload):
            ComBlockExprAST(ComLineBlockExprAST::Type::COM_LINE_BLOCK_EXPR),
            stmtList(std::move(stmtList)), payload(std::move(payload)) {}
    const std::unique_ptr<StmtLstExprAST> &GetStmtList();
    const std::unique_ptr<PayloadExprAST> &GetPayload();
};
