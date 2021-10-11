/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "Token.h"

Token::Token() = default;

Token::Token(int type, unsigned int lineNum, unsigned int colNum) {
    this->type = type;
    this->lineNum = lineNum;
    this->colNum = colNum;
}

Token::Token(int type, std::string value, unsigned int lineNum, unsigned int colNum) {
    this->type = type;
    this->value = std::move(value);
    this->lineNum = lineNum;
    this->colNum = colNum;
}

int Token::getType() const {
    return type;
}

std::string Token::getValue() {
    return value;
}

std::string Token::getCodePos() {
    return "Line " + std::to_string(lineNum) + ", Col " + std::to_string(colNum);
}