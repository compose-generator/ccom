// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <vector>
#include <memory>
#include "ComBlockExprAST.h"
#include "ContentBlockExprAST.h"

class SectionExprAST : public ContentBlockExprAST {
public:
    // Constructors
    explicit SectionExprAST(std::vector<std::unique_ptr<ComBlockExprAST>> comBlocks):
            ContentBlockExprAST(ContentBlockExprType::SECTION_EXPR), comBlocks(std::move(comBlocks)) {}

    // Public methods
    const std::vector<std::unique_ptr<ComBlockExprAST>> &getComBlocks();
private:
    // Members
    std::vector<std::unique_ptr<ComBlockExprAST>> comBlocks;
};
