#pragma once

#include <memory>
#include "ComBlockExprAST.h"
#include "IfBlockExprAST.h"

class ComLineBlockExprAST : public ComBlockExprAST {
public:
    // Constructors
    explicit ComLineBlockExprAST(std::unique_ptr<IfBlockExprAST> ifBlock):
            ComBlockExprAST(ComBlockExprType::COM_LINE_BLOCK_EXPR), ifBlock(std::move(ifBlock)) {}

    // Public methods
    const std::unique_ptr<IfBlockExprAST> &getIfBlock();
    std::string serialize() const;
private:
    // Members
    std::unique_ptr<IfBlockExprAST> ifBlock;
};
