// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#include "ContentExprAST.h"

std::vector<std::unique_ptr<ExprAST>> const &ContentExprAST::GetSections() {
    return Sections;
}