// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

#include <memory>
#include "StmtLstExprAST.h"
#include "PayloadExprAST.h"

class IfBlockExprAST {
public:
    // Constructors
    explicit IfBlockExprAST(std::unique_ptr<StmtLstExprAST> stmtList, std::unique_ptr<PayloadExprAST> payload):
        stmtList(std::move(stmtList)), payload(std::move(payload)) {}

    // Public methods
    const std::unique_ptr<StmtLstExprAST> &getStmtList();
    const std::unique_ptr<PayloadExprAST> &getPayload();
private:
    // Members
    std::unique_ptr<StmtLstExprAST> stmtList;
    std::unique_ptr<PayloadExprAST> payload;
};
