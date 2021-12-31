/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#include "ContainsStmtExprAST.h"

Operator ContainsStmtExprAST::getOperator() const {
    return op;
}

std::unique_ptr<KeyExprAST> const &ContainsStmtExprAST::getListKey() {
    return listKey;
}

std::unique_ptr<KeyExprAST> const &ContainsStmtExprAST::getValueKey() {
    return valueKey;
}

std::unique_ptr<ValueExprAST> const &ContainsStmtExprAST::getValue() {
    return value;
}

bool ContainsStmtExprAST::getInverted() const {
    return isInverted;
};

std::string ContainsStmtExprAST::serialize() const {
    return "contains-stmt(list-key: " + listKey->serialize() + ", value-key: " + valueKey->serialize() +
        ", operator: " + std::to_string(op) + ", value: " + value->serialize() + ")";
}