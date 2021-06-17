// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

#include <string>

class Token {
public:
    // Constructors
    Token();
    Token(int, unsigned int, unsigned int);
    Token(int, std::string, unsigned int, unsigned int);

    // Public methods
    int getType() const;
    std::string getValue();
    std::string getCodePos();
private:
    // Members
    int type = -1;
    std::string value;
    unsigned int lineNum = 0;
    unsigned int colNum = 0;
};

enum TokenType {
    // End of file
    TOK_EOF,

    // Keywords
    TOK_IF, // if
    TOK_HAS, // has
    TOK_NOT, // not
    TOK_CONTAINS, // contains

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

    // The following examples refer to Java
    TOK_COM_IDEN_PAYLOAD, // //
    TOK_COM_LINE_IDEN, // //?
    TOK_COM_BLOCK_IDEN_OPEN, // /*?
    TOK_COM_BLOCK_IDEN_CLOSE, // */

    TOK_ARBITRARY // e.g. asd'!?fowen7a_=sdfkh%"
};