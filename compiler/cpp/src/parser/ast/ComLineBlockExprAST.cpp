/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "ComLineBlockExprAST.h"

const std::unique_ptr<IfBlockExprAST> &ComLineBlockExprAST::getIfBlock() {
    return ifBlock;
}

std::string ComLineBlockExprAST::serialize() const {
    return "com-line-block(" + ifBlock->serialize() + ")";
}
