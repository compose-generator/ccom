// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#pragma once

#include <string>

class Token {
private:
    int type = 0;
    std::string value;
    unsigned int lineNum = 0;
    unsigned int colNum = 0;
public:
    Token();
    Token(int, unsigned int, unsigned int);
    Token(int, std::string, unsigned int, unsigned int);
    int getType() const;
    std::string getValue();
    std::string getCodePos();
};

enum TokenType {
    // Unknown token type
    TOK_UNKNOWN,

    // End of file
    TOK_EOF,

    // Keywords
    TOK_IF, // if
    TOK_HAS, // has
    TOK_NOT, // not

    // Boolean values
    TOK_TRUE, // true
    TOK_FALSE, // false

    // Operators
    TOK_OR, // |
    TOK_EQUALS, // ==
    TOK_NOT_EQUALS, // !=
    TOK_GREATER, // >
    TOK_LESS, // <
    TOK_GREATER_EQUAL, // >=
    TOK_LESS_EQUAL, // <=

    // Misc
    TOK_IDENTIFIER, // e.g. test
    TOK_NUMBER, // e.g. 123
    TOK_STRING, // "test"
    TOK_DOT, // .
    TOK_BRACE_OPEN, // {
    TOK_BRACE_CLOSE, // }
    TOK_INDEX, // [123]
    TOK_COM_IDEN_PAYLOAD, // //
    TOK_COM_LINE_IDEN, // //?
    TOK_COM_BLOCK_IDEN_OPEN, // /*?
    TOK_COM_BLOCK_IDEN_CLOSE, // */

    TOK_ARBITRARY // e.g. asd'!?fowen7a_=sdfkh%"
};