// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_HASSTMTEXPRAST_H
#define COMPILER_HASSTMTEXPRAST_H

#include <memory>
#include "KeyExprAST.h"
#include "StmtExprAST.h"

class HasStmtExprAST : public StmtExprAST {
private:
    std::unique_ptr<KeyExprAST> Key;
    bool Inverted;
public:
    explicit HasStmtExprAST(std::unique_ptr<KeyExprAST> key, bool inverted):
            Key(std::move(key)), Inverted(inverted) {}
    const std::unique_ptr<KeyExprAST> &GetKey();
    bool GetInverted() const;
};

#endif //COMPILER_HASSTMTEXPRAST_H
