// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 09.05.2021.
//

#include "ComBlockBlockExprAST.h"

std::unique_ptr<IfBlockExprAST> const &ComBlockBlockExprAST::getIfBlock() {
    return ifBlock;
}

std::string ComBlockBlockExprAST::serialize() const {
    return "com-block-block(" + ifBlock->serialize() + ")";
}