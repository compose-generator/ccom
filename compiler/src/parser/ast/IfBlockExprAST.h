// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_IFBLOCKEXPRAST_H
#define COMPILER_IFBLOCKEXPRAST_H

#include <memory>
#include "StmtLstExprAST.h"
#include "PayloadExprAST.h"

class IfBlockExprAST {
private:
    std::unique_ptr<StmtLstExprAST> StmtList;
    std::unique_ptr<PayloadExprAST> Payload;
public:
    explicit IfBlockExprAST(std::unique_ptr<StmtLstExprAST> stmtList, std::unique_ptr<PayloadExprAST> payload):
        StmtList(std::move(stmtList)), Payload(std::move(payload)) {}
    const std::unique_ptr<StmtLstExprAST> &GetStmtList();
    const std::unique_ptr<PayloadExprAST> &GetPayload();
};

#endif //COMPILER_IFBLOCKEXPRAST_H
