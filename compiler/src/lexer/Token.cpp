//
// Created by Marc on 02.05.2021.
//

#include "Token.h"

#include <utility>

Token::Token() = default;

Token::Token(int type) {
    this->type = type;
}

Token::Token(int type, std::string value) {
    this->type = type;
    this->value = std::move(value);
}

int Token::getType() {
    return type;
}

std::string Token::getValue() {
    return value;
}