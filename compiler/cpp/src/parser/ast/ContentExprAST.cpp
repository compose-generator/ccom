/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#include "ContentExprAST.h"

std::vector<std::unique_ptr<ContentBlockExprAST>> const &ContentExprAST::getContentBlocks() {
    return contentBlocks;
}

std::string ContentExprAST::serialize() const {
    std::string serializedContentBlocks;
    for (auto& contentBlock : contentBlocks) {
        serializedContentBlocks += contentBlock->serialize();
    }
    return "content(" + serializedContentBlocks + ")";
}
