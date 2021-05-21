// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <utility>
#include <vector>
#include <memory>
#include "StmtExprAST.h"

class StmtLstExprAST : public ExprAST {
private:
    std::vector<std::unique_ptr<StmtExprAST>> Stmts;
public:
    explicit StmtLstExprAST(std::vector<std::unique_ptr<StmtExprAST>> stmts): Stmts(std::move(stmts)) {}
    const std::vector<std::unique_ptr<StmtExprAST>> &GetStatements();
};
