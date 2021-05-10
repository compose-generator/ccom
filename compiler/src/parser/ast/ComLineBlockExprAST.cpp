// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#include "ComLineBlockExprAST.h"

std::unique_ptr<StmtLstExprAST> const &ComLineBlockExprAST::GetStmtList() {
    return StmtList;
}

std::unique_ptr<PayloadExprAST> const &ComLineBlockExprAST::GetPayload() {
    return Payload;
}