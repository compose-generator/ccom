// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <memory>
#include "IfBlockExprAST.h"
#include "ComBlockExprAST.h"

class ComBlockBlockExprAST: public ComBlockExprAST {
private:
    std::unique_ptr<IfBlockExprAST> ifBlock;
public:
    explicit ComBlockBlockExprAST(std::unique_ptr<IfBlockExprAST> ifBlock): ifBlock(std::move(ifBlock)) {}
    const std::unique_ptr<IfBlockExprAST> &GetIfBlock();
};
