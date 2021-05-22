// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <utility>
#include <vector>
#include <memory>
#include "StmtExprAST.h"
#include "TopLevelExprAST.h"

class StmtLstExprAST: public TopLevelExprAST {
private:
    std::vector<std::unique_ptr<StmtExprAST>> stmts;
public:
    explicit StmtLstExprAST(std::vector<std::unique_ptr<StmtExprAST>> stmts):
            TopLevelExprAST(TopLevelExprAST::Type::STMT_LST_EXPR), stmts(std::move(stmts)) {}
    const std::vector<std::unique_ptr<StmtExprAST>> &GetStatements();
};
