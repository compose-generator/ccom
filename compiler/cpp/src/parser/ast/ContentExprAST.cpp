// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "ContentExprAST.h"

std::vector<std::unique_ptr<ContentBlockExprAST>> const &ContentExprAST::getContentBlocks() {
    return contentBlocks;
}