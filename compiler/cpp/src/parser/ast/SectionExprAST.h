/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

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
    std::string serialize() const;
private:
    // Members
    std::vector<std::unique_ptr<ComBlockExprAST>> comBlocks;
};
