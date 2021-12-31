/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "StringExprAST.h"

std::string StringExprAST::getValue() const {
    return value;
}

std::string StringExprAST::serialize() const {
    return "string(value: '" + value + "')";
}
