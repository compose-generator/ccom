// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#include "SectionExprAST.h"

std::vector<std::unique_ptr<ComBlockExprAST>> const &SectionExprAST::GetComBlocks() {
    return ComBlocks;
}