//
// Created by Marc on 02.05.2021.
//

#include "Token.h"

#include <utility>

Token::Token() = default;

Token::Token(int type) {
    this->type = type;
}

Token::Token(int type, int valueInt) {
    this->type = type;
    this->intValue = valueInt;
}

Token::Token(int type, std::string valueString) {
    this->type = type;
    this->stringValue = std::move(valueString);
}

int Token::getType() {
    return type;
}

int Token::getIntValue() {
    return intValue;
}

std::string Token::getStringValue() {
    return stringValue;
}