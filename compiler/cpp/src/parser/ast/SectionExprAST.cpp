/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "SectionExprAST.h"

std::vector<std::unique_ptr<ComBlockExprAST>> const &SectionExprAST::getComBlocks() {
    return comBlocks;
}

std::string SectionExprAST::serialize() const {
    std::string serializedComBlocks;
    for (auto& comBlock : comBlocks) {
        serializedComBlocks += comBlock->serialize();
    }
    return "section(" + serializedComBlocks + ")";
}