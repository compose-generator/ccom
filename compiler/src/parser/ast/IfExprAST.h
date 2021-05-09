// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_IFEXPRAST_H
#define COMPILER_IFEXPRAST_H

#include <memory>
#include "StmtLstExprAST.h"
#include "PayloadExprAST.h"

class IfExprAST {
private:
    std::unique_ptr<StmtLstExprAST> StmtList;
    std::unique_ptr<PayloadExprAST> Payload;
public:
    explicit IfExprAST(std::unique_ptr<StmtLstExprAST> stmtList, std::unique_ptr<PayloadExprAST> payload):
        StmtList(std::move(stmtList)), Payload(std::move(payload)) {}
};

#endif //COMPILER_IFEXPRAST_H
