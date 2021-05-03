//
// Created by Marc on 02.05.2021.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include "Token.h"

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

    // Misc
    TOK_IDENTIFIER, // e.g. test
    TOK_NUMBER, // e.g. 123
    TOK_STRING, // "test"
    TOK_DOT, // .
    TOK_BRACE_OPEN, // {
    TOK_BRACE_CLOSE, // }
    TOK_COM_LINE_IDEN, // //?
    TOK_COM_BLOCK_IDEN_OPEN, // /*?
    TOK_COM_BLOCK_IDEN_CLOSE, // */
    TOK_COM_IDEN_PAYLOAD, // //??
};

int advance();

int expect(int);

std::string getLookahead();

Token getTok();

void initLexer(std::string, const std::string&, const std::string&, std::string);

#endif //COMPILER_LEXER_H