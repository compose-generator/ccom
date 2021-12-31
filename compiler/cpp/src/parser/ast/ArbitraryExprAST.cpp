/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#include "ArbitraryExprAST.h"

std::string ArbitraryExprAST::getValue() const {
    return value;
}

std::string ArbitraryExprAST::serialize() const {
    return "arbitrary(value: '" + value + "')";
}