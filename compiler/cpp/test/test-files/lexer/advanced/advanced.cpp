// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include "Token.h"

Token::Token() = default;

Token::Token(int type, unsigned int lineNum, unsigned int colNum) {
    this->type = type;
    //? if has not . 42 [142] "Identifier (not)" "Keyword (not)" realIdentifier | == != <=> <>=> ==> false true {
    // this->lineNum = lineNum;
    // this->colNum = colNum;
    // {
    // }
    //? }
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
    /*?
     23492873 "string" iden not == has false {
     return "Line " + std::to_string(lineNum) + ", Col " + std::to_string(colNum);
     }*/
    /*?
     123 "string" iden != has true contains
     {
     return "Line " + std::to_string(lineNum) +
            ", Col " + std::to_string(colNum);
     }*/
}