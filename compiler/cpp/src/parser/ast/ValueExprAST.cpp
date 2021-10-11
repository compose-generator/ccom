/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "ValueExprAST.h"
#include "NumberExprAST.h" // This import cant be in the header file for some reason
#include "BooleanExprAST.h" // This import cant be in the header file for some reason
#include "StringExprAST.h" // This import cant be in the header file for some reason

ValueExprType ValueExprAST::getType() {
    return type;
}

std::string ValueExprAST::serialize() {
    switch(type) {
        case NUMBER_EXPR: {
            auto* numberExpr = static_cast<NumberExprAST*>(this);
            return numberExpr->serialize();
        }
        case BOOLEAN_EXPR: {
            auto* booleanExpr = static_cast<BooleanExprAST*>(this);
            return booleanExpr->serialize();
        }
        case STRING_EXPR: {
            auto* stringExprAST = static_cast<StringExprAST*>(this);
            return stringExprAST->serialize();
        }
        default:
            throw std::runtime_error("Unknown value type");
    }
}