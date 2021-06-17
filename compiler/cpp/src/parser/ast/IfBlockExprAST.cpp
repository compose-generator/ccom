// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#include "IfBlockExprAST.h"

std::unique_ptr<StmtLstExprAST> const &IfBlockExprAST::getStmtList() {
    return stmtList;
}

std::unique_ptr<PayloadExprAST> const &IfBlockExprAST::getPayload() {
    return payload;
}

std::string IfBlockExprAST::serialize() const {
    return "if(stmt-lst: " + stmtList.serialize() + ", payload: " + payload.serialize() + ")";
}