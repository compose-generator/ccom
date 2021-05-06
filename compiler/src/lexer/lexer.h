// Copyright (c) Marc Auberer 2021. All rights reserved.

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
    TOK_COM_IDEN_PAYLOAD, // //
    TOK_COM_LINE_IDEN, // //?
    TOK_COM_BLOCK_IDEN_OPEN, // /*?
    TOK_COM_BLOCK_IDEN_CLOSE, // */

    TOK_ARBITRARY // e.g. asd'!?fowen7a_=sdfkh%"
};

enum Context {
    ARBITRARY, // Before / after conditional section
    SECTION, // Conditional section itself
    PAYLOAD // Payload content
};

int advance();

int expect(int);

std::string getLookahead();

Token getTok();

Token consumeStringLiteral();

Token consumeIdentifierOrKeyword();

bool isEOF();

void skipWhitespaces();

Token consumeNumber();

Token consumeArbitrary();

Token consumePayload();

bool isLookaheadPayloadCommentChars();

bool isLookaheadLineCommentChars();

bool isLookaheadBlockCommentCharOpen();

bool isLookaheadBlockCommentCharClose();

void initLexer(const std::string&, const std::string&, const std::string&, const std::string&);

#endif //COMPILER_LEXER_H