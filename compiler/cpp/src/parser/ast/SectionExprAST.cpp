// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "SectionExprAST.h"

std::vector<std::unique_ptr<ComBlockExprAST>> const &SectionExprAST::getComBlocks() {
    return comBlocks;
}