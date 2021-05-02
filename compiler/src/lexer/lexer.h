//
// Created by Marc on 02.05.2021.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include "Token.h"

enum TokenType {
    TOK_UNKNOWN                     = 0,

    // End of file
    TOK_EOF                         = -1,

    // Keywords
    TOK_IF                          = -2, // if
    TOK_HAS                         = -3, // has
    TOK_NOT                         = -4, // not

    // Boolean values
    TOK_TRUE                        = -5, // true
    TOK_FALSE                       = -6, // false

    // Operators
    TOK_OR                          = -7, // |
    TOK_EQUALS                      = -8, // ==
    TOK_NOT_EQUALS                  = -9, // !=

    // Misc
    TOK_IDENTIFIER                  = -10, // e.g. test
    TOK_NUMBER                      = -11, // e.g. 123
    TOK_STRING                      = -12, // "test"
    TOK_DOT                         = -13, // .
    TOK_BRACE_OPEN                  = -14, // {
    TOK_BRACE_CLOSE                 = -15, // }
    TOK_COM_LINE_IDEN               = -16, // //!
    TOK_COM_BLOCK_IDEN_OPEN         = -17, // /*!
    TOK_COM_BLOCK_IDEN_CLOSE        = -18, // */
    TOK_COM_IDEN_PAYLOAD            = -19, // //!!
};

int advance();

int expect(int);

int variableLookahead(int);

Token getTok();

void initLexer(std::string, const std::string&, const std::string&, std::string);

#endif //COMPILER_LEXER_H