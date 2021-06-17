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
#include "HasStmtExprAST.h"
#include "CompStmtExprAST.h"

class StmtLstExprAST: public TopLevelExprAST {
public:
    // Constructors
    explicit StmtLstExprAST(std::vector<std::unique_ptr<StmtExprAST>> stmts):
            TopLevelExprAST(TopLevelExprType::STMT_LST_EXPR), stmts(std::move(stmts)) {}

    // Public methods
    const std::vector<std::unique_ptr<StmtExprAST>> &getStatements();
    std::string serialize() const;
private:
    // Members
    std::vector<std::unique_ptr<StmtExprAST>> stmts;
};
