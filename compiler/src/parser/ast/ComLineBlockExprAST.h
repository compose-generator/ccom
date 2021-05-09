// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_COMLINEBLOCKEXPRAST_H
#define COMPILER_COMLINEBLOCKEXPRAST_H

#include <memory>
#include "ExprAST.h"
#include "StmtLstExprAST.h"
#include "PayloadExprAST.h"
#include "ComBlockExprAST.h"

class ComLineBlockExprAST : public ComBlockExprAST {
private:
    std::unique_ptr<StmtLstExprAST> StmtList;
    std::unique_ptr<PayloadExprAST> Payload;
public:
    explicit ComLineBlockExprAST(std::unique_ptr<StmtLstExprAST> stmtList, std::unique_ptr<PayloadExprAST> payload):
            StmtList(std::move(stmtList)), Payload(std::move(payload)) {}
};

#endif //COMPILER_COMLINEBLOCKEXPRAST_H
