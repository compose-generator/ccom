/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#include "ComBlockExprAST.h"
#include "ComLineBlockExprAST.h" // This import cant be in the header file for some reason
#include "ComBlockBlockExprAST.h" // This import cant be in the header file for some reason

ComBlockExprType ComBlockExprAST::getType() {
    return type;
}

std::string ComBlockExprAST::serialize() {
    switch (type) {
        case COM_LINE_BLOCK_EXPR: {
            auto* comLineBlock = static_cast<ComLineBlockExprAST*>(this);
            return comLineBlock->serialize();
        }
        case COM_BLOCK_BLOCK_EXPR: {
            auto* comBlockBlock = static_cast<ComBlockBlockExprAST*>(this);
            return comBlockBlock->serialize();
        }
        default:
            throw std::runtime_error("Unknown com block type");
    }
}