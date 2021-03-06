/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>
#include <stdexcept>

enum ComBlockExprType {
    COM_BLOCK_EXPR,
    COM_LINE_BLOCK_EXPR,
    COM_BLOCK_BLOCK_EXPR
};

class ComBlockExprAST {
public:
    // Constructors
    explicit ComBlockExprAST(): type(COM_BLOCK_EXPR) {}

    // Public methods
    ComBlockExprType getType();
    std::string serialize();
protected:
    // Protected constructors
    explicit ComBlockExprAST(ComBlockExprType t): type(t) {}
private:
    // Members
    ComBlockExprType type;
};