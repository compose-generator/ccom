// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "SectionExprAST.h"
#include "TopLevelExprAST.h"

class ContentExprAST : public TopLevelExprAST {
public:
    // Constructors
    explicit ContentExprAST(std::vector<std::unique_ptr<ContentBlockExprAST>> sections):
        TopLevelExprAST(TopLevelExprType::CONTENT_EXPR), contentBlocks(std::move(sections)) {}

    // Public methods
    const std::vector<std::unique_ptr<ContentBlockExprAST>> &getContentBlocks();
private:
    // Members
    std::vector<std::unique_ptr<ContentBlockExprAST>> contentBlocks;
};
