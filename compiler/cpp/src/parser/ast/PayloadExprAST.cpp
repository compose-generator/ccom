/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "PayloadExprAST.h"

std::string PayloadExprAST::getValue() const {
    return value;
}

std::string PayloadExprAST::serialize() const {
    return "payload(value: '" + value + "')";
}