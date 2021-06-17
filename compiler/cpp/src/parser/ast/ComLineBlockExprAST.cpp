// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#include "ComLineBlockExprAST.h"

std::unique_ptr<StmtLstExprAST> const &ComLineBlockExprAST::getStmtList() {
    return stmtList;
}

std::unique_ptr<PayloadExprAST> const &ComLineBlockExprAST::getPayload() {
    return payload;
}

std::string ComLineBlockExprAST::serialize() const {
    return "com-line-block(stmt-lst: " + stmtList.serialize() + ", payload: " + payload->serialize() + ")";
}