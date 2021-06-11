// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

enum TopLevelExprType {
    TOP_LEVEL_EXPR,
    CONTENT_EXPR,
    STMT_LST_EXPR
};

class TopLevelExprAST {
public:
    // Constructors
    explicit TopLevelExprAST(): type(TOP_LEVEL_EXPR) {}

    // Public methods
    TopLevelExprType getType();
protected:
    // Protected constructors
    explicit TopLevelExprAST(TopLevelExprType t): type(t) {}
private:
    // Members
    TopLevelExprType type;
};
