//
// Created by Marc on 22.05.2021.
//

#pragma once

enum ContentBlockExprType {
    CONTENT_BLOCK_EXPR,
    SECTION_EXPR,
    ARBITRARY_EXPR
};

class ContentBlockExprAST {
public:
    // Constructors
    explicit ContentBlockExprAST(): type(CONTENT_BLOCK_EXPR) {}

    // Public methods
    ContentBlockExprType getType();
protected:
    // Protected constructors
    explicit ContentBlockExprAST(ContentBlockExprType t): type(t) {}
private:
    // Members
    ContentBlockExprType type;
};
