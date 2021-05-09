// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_COMBLOCKBLOCKEXPRAST_H
#define COMPILER_COMBLOCKBLOCKEXPRAST_H

#include <vector>
#include <memory>
#include "ExprAST.h"
#include "IfExprAST.h"

class ComBlockBlockExprAST: public ExprAST {
private:
    std::vector<std::unique_ptr<IfExprAST>> IfBlocks;
public:
    explicit ComBlockBlockExprAST(std::vector<std::unique_ptr<IfExprAST>> ifBlocks): IfBlocks(std::move(ifBlocks)) {}
};

#endif //COMPILER_COMBLOCKBLOCKEXPRAST_H
