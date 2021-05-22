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
private:
    std::vector<std::unique_ptr<ContentBlockExprAST>> sections;
public:
    explicit ContentExprAST(std::vector<std::unique_ptr<ContentBlockExprAST>> sections):
        TopLevelExprAST(TopLevelExprAST::Type::CONTENT_EXPR), sections(std::move(sections)) {}
    const std::vector<std::unique_ptr<ContentBlockExprAST>> &GetSections();
};
