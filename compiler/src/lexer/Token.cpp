// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

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