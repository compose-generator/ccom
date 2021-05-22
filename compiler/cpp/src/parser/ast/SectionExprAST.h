// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <vector>
#include <memory>
#include "ExprAST.h"
#include "ComBlockExprAST.h"

class SectionExprAST : public ExprAST {
private:
    std::vector<std::unique_ptr<ComBlockExprAST>> comBlocks;
public:
    explicit SectionExprAST(std::vector<std::unique_ptr<ComBlockExprAST>> comBlocks):
            comBlocks(std::move(comBlocks)) {}
    const std::vector<std::unique_ptr<ComBlockExprAST>> &GetComBlocks();
};
