//
// Created by Marc on 02.05.2021.
//

#include "Token.h"

#include <utility>

Token::Token() = default;

Token::Token(int type, int lineNum, int colNum) {
    this->type = type;
    this->lineNum = lineNum;
    this->colNum = colNum;
}

Token::Token(int type, std::string value, int lineNum, int colNum) {
    this->type = type;
    this->value = std::move(value);
    this->lineNum = lineNum;
    this->colNum = colNum;
}

int Token::getType() {
    return type;
}

std::string Token::getValue() {
    return value;
}

std::string Token::getCodePos() {
    return "Line " + std::to_string(lineNum) + ", Col " + std::to_string(colNum);
}