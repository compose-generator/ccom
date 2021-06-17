// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "ComLineBlockExprAST.h"

std::unique_ptr<StmtLstExprAST> const &ComLineBlockExprAST::getStmtList() {
    return stmtList;
}

std::unique_ptr<PayloadExprAST> const &ComLineBlockExprAST::getPayload() {
    return payload;
}