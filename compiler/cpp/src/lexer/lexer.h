// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#pragma once

#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "Token.h"

enum Context {
    ARBITRARY, // Before / after conditional section
    SECTION, // Conditional section itself
    PAYLOAD // payload content
};

int advance();

void expect(int);

std::string getLookahead();

Token getTok();

Token consumeStringLiteral();

Token consumeIdentifierOrKeyword();

bool isEOF();

void skipWhitespaces();

std::string consumeNumber();

Token consumeArbitrary();

Token consumePayload();

bool isLookaheadPayloadCommentChars();

bool isLookaheadLineCommentChars();

bool isLookaheadBlockCommentCharOpen();

bool isLookaheadBlockCommentCharClose();

bool isLookaheadBlockCommentCharCloseWithBrace();

void initLexer(bool, const std::string&, const std::string&, const std::string&, const std::string&);