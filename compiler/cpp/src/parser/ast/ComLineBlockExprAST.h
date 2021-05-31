// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <memory>
#include "StmtLstExprAST.h"
#include "PayloadExprAST.h"
#include "ComBlockExprAST.h"

class ComLineBlockExprAST : public ComBlockExprAST {
public:
    // Constructors
    explicit ComLineBlockExprAST(std::unique_ptr<StmtLstExprAST> stmtList, std::unique_ptr<PayloadExprAST> payload):
            ComBlockExprAST(ComBlockExprType::COM_LINE_BLOCK_EXPR),
            stmtList(std::move(stmtList)), payload(std::move(payload)) {}

    // Public methods
    const std::unique_ptr<StmtLstExprAST> &getStmtList();
    const std::unique_ptr<PayloadExprAST> &getPayload();
private:
    // Members
    std::unique_ptr<StmtLstExprAST> stmtList;
    std::unique_ptr<PayloadExprAST> payload;
};
