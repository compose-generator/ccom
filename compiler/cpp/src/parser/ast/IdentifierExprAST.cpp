/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "IdentifierExprAST.h"

std::string IdentifierExprAST::getName() const {
    return name;
}

int IdentifierExprAST::getIndex() const {
    return index;
}

std::string IdentifierExprAST::serialize() const {
    return "identifier(name: '" + name + "', index: '" + std::to_string(index) + "')";
}