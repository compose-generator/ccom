// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

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
    std::string serialize() const;
private:
    // Members
    std::unique_ptr<StmtLstExprAST> stmtList;
    std::unique_ptr<PayloadExprAST> payload;
};
