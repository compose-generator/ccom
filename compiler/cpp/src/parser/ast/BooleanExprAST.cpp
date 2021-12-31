/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#include "BooleanExprAST.h"

bool BooleanExprAST::getValue() const {
    return value;
}

std::string BooleanExprAST::serialize() const {
    return "boolean(value: '" + std::to_string(value) + "')";
}
