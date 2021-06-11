//
// Created by Marc on 20.05.2021.
//

#include "BooleanExprAST.h"

bool BooleanExprAST::getValue() const {
    return value;
}

std::string BooleanExprAST::serialize() const {
    return "boolean(value: '" + std::to_string(value) + "')";
}
