// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_COMBLOCKBLOCKEXPRAST_H
#define COMPILER_COMBLOCKBLOCKEXPRAST_H

#include <memory>
#include "IfExprAST.h"
#include "ComBlockExprAST.h"

class ComBlockBlockExprAST: public ComBlockExprAST {
private:
    std::unique_ptr<IfExprAST> IfBlock;
public:
    explicit ComBlockBlockExprAST(std::unique_ptr<IfExprAST> ifBlock): IfBlock(std::move(ifBlock)) {}
};

#endif //COMPILER_COMBLOCKBLOCKEXPRAST_H
