#pragma once

#include <memory>
#include "IfBlockExprAST.h"
#include "ComBlockExprAST.h"

class ComBlockBlockExprAST: public ComBlockExprAST {
public:
    // Constructors
    explicit ComBlockBlockExprAST(std::unique_ptr<IfBlockExprAST> ifBlock):
        ComBlockExprAST(ComBlockExprType::COM_BLOCK_BLOCK_EXPR), ifBlock(std::move(ifBlock)) {}

    // Public methods
    const std::unique_ptr<IfBlockExprAST> &getIfBlock();
    std::string serialize() const;
private:
    // Members
    std::unique_ptr<IfBlockExprAST> ifBlock;
};
