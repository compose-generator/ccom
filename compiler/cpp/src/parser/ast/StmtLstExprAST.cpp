// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "StmtLstExprAST.h"

std::vector<std::unique_ptr<StmtExprAST>> const &StmtLstExprAST::getStatements() {
    return stmts;
}