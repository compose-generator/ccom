/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#include "Token.h"

Token::Token(TokenType type, unsigned int lineNum, unsigned int colNum) {
  this->type = type;
  this->lineNum = lineNum;
  this->colNum = colNum;
}

Token::Token(TokenType type, std::string value, unsigned int lineNum, unsigned int colNum) {
  this->type = type;
  this->value = std::move(value);
  this->lineNum = lineNum;
  this->colNum = colNum;
}

TokenType Token::getType() const { return type; }

std::string Token::getValue() { return value; }

std::string Token::getCodePos() const { return "Line " + std::to_string(lineNum) + ", Col " + std::to_string(colNum); }