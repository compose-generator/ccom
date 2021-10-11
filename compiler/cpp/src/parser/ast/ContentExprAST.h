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
    std::string serialize() const;
private:
    // Members
    std::vector<std::unique_ptr<ContentBlockExprAST>> contentBlocks;
};
