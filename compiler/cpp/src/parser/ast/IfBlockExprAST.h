// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <memory>
#include "StmtLstExprAST.h"
#include "PayloadExprAST.h"

class IfBlockExprAST {
private:
    std::unique_ptr<StmtLstExprAST> stmtList;
    std::unique_ptr<PayloadExprAST> payload;
public:
    explicit IfBlockExprAST(std::unique_ptr<StmtLstExprAST> stmtList, std::unique_ptr<PayloadExprAST> payload):
        stmtList(std::move(stmtList)), payload(std::move(payload)) {}
    const std::unique_ptr<StmtLstExprAST> &GetStmtList();
    const std::unique_ptr<PayloadExprAST> &GetPayload();
};
