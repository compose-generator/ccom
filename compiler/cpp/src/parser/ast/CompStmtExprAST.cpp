// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "CompStmtExprAST.h"

Operator CompStmtExprAST::getOperator() const {
    return op;
}

std::unique_ptr<KeyExprAST> const &CompStmtExprAST::getKey() {
    return key;
}

std::unique_ptr<ValueExprAST> const &CompStmtExprAST::getValue() {
    return value;
}

std::string CompStmtExprAST::serialize() const {
    std::string serializedValue;
    switch(value->getType()) {
        case NUMBER_EXPR: {
            auto* numberExpr = static_cast<NumberExprAST*>(value.get());
            serializedValue = numberExpr->serialize();
            break;
        }
        case BOOLEAN_EXPR: {
            auto* booleanExpr = static_cast<BooleanExprAST*>(value.get());
            serializedValue = booleanExpr->serialize();
            break;
        }
        case STRING_EXPR: {
            auto* stringExprAST = static_cast<NumberExprAST*>(value.get());
            serializedValue = stringExprAST->serialize();
            break;
        }
        default:
            throw std::runtime_error("Unknown value type");
    }
    return "comp-stmt(key: " + key->serialize() + ", operator: " + std::to_string(op) + ", value: " + serializedValue + ")";
}