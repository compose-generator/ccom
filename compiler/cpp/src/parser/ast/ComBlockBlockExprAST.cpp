/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "ComBlockBlockExprAST.h"

std::unique_ptr<IfBlockExprAST> const &ComBlockBlockExprAST::getIfBlock() {
    return ifBlock;
}

std::string ComBlockBlockExprAST::serialize() const {
    return "com-block-block(" + ifBlock->serialize() + ")";
}