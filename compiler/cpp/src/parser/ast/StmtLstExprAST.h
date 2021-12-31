/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <utility>
#include <vector>
#include <memory>
#include "StmtExprAST.h"
#include "TopLevelExprAST.h"

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
