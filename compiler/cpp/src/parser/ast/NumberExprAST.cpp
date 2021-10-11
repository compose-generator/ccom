/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "NumberExprAST.h"

int NumberExprAST::getValue() const {
    return value;
}

std::string NumberExprAST::serialize() const {
    return "number(value: '" + std::to_string(value) + "')";
}
