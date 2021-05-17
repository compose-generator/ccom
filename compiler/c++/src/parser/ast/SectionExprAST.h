// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#ifndef COMPILER_SECTIONEXPRAST_H
#define COMPILER_SECTIONEXPRAST_H

#include <vector>
#include <memory>
#include "ExprAST.h"
#include "ComBlockExprAST.h"

class SectionExprAST : public ExprAST {
private:
    std::vector<std::unique_ptr<ComBlockExprAST>> ComBlocks;
public:
    explicit SectionExprAST(std::vector<std::unique_ptr<ComBlockExprAST>> comBlocks):
            ComBlocks(std::move(comBlocks)) {}
    const std::vector<std::unique_ptr<ComBlockExprAST>> &GetComBlocks();
};

#endif //COMPILER_SECTIONEXPRAST_H
