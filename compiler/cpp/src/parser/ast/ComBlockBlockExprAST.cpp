/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "ComBlockBlockExprAST.h"

std::unique_ptr<IfBlockExprAST> const &ComBlockBlockExprAST::getIfBlock() {
    return ifBlock;
}

std::string ComBlockBlockExprAST::serialize() const {
    return "com-block-block(" + ifBlock->serialize() + ")";
}