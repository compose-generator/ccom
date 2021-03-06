/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <memory>
#include "KeyExprAST.h"
#include "StmtExprAST.h"

class HasStmtExprAST : public StmtExprAST {
public:
    // Constructors
    explicit HasStmtExprAST(std::unique_ptr<KeyExprAST> key, bool inverted):
        StmtExprAST(StmtExprType::HAS_STMT_EXPR), key(std::move(key)), isInverted(inverted) {}

    // Public methods
    const std::unique_ptr<KeyExprAST> &getKey();
    bool getInverted() const;
    std::string serialize() const;
private:
    // Members
    std::unique_ptr<KeyExprAST> key;
    bool isInverted;
};