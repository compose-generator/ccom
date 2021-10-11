/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "Token.h"
#include "../reader/Reader.h"
#include "../exception/UnexpectedTokenException.h"

enum Context {
    ARBITRARY, // Before / after conditional section
    SECTION, // Conditional section itself
    PAYLOAD // Payload content
};

class Lexer {
public:
    // Constructors
    explicit Lexer(): currentContext(ARBITRARY) {};
    Lexer(bool, const std::string&, const std::string&, const std::string&, const std::string&);

    // Public methods
    void advance();
    void expect(TokenType);
    Token getLookahead();
private:
    // Private methods
    Token consumeArbitrary();
    Token consumePayload();
    Token consumeSection();
    Token consumeEOF();
    Token consumeOr();
    Token consumeEquals();
    Token consumeNotEquals();
    Token consumeGreaterOrGreaterEqual();
    Token consumeLessOrLessEqual();
    Token consumeDot();
    Token consumeBraceOpen();
    Token consumeBraceClose();
    Token consumeIndex();
    Token consumeString();
    Token consumeIdentifierOrKeyword();
    Token consumeNumber();
    Token consumeCommentLineIdentifier();
    Token consumeCommentBlockOpenIdentifier();
    Token consumeCommentBlockCloseIdentifier();
    Token consumeCommentPayloadIdentifier();

    bool isEOF();
    bool isLookAheadCommentLineIdentifier();
    bool isLookAheadCommentBlockOpenIdentifier();
    bool isLookAheadCommentBlockCloseIdentifier();
    bool isLookAheadCommentPayloadIdentifier();
    bool isLookAheadCommentBlockCloseIdentifierWithBrace();

    Token constructToken(TokenType);
    Token constructToken(TokenType, std::string);
    void updateTokenStartPosition();

    // Members
    std::string commentLineIdentifier;
    std::string commentBlockOpenIdentifier;
    std::string commentBlockCloseIdentifier;
    std::string commentPayloadIdentifier;
    Reader reader;
    Context currentContext;
    Token curTok;
    unsigned int tokenStartLineNum;
    unsigned int tokenStartColNum;
};