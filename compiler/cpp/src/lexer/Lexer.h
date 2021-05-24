//
// Created by Marc on 23.05.2021.
//

#pragma once

#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "Token.h"
#include "../reader/Reader.h"

enum Context {
    ARBITRARY, // Before / after conditional section
    SECTION, // Conditional section itself
    PAYLOAD // Payload content
};

class Lexer {
private:
    // Constant inputs
    std::string commentLineIdentifier;
    std::string commentBlockOpenIdentifier;
    std::string commentBlockCloseIdentifier;
    std::string commentPayloadIdentifier;

    // Working variables
    Reader reader;
    Context currentContext;
    Token curTok;

    // Private functions
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
public:
    explicit Lexer(): currentContext(ARBITRARY) {};
    Lexer(bool isSingleStatement, const std::string& fileInput, const std::string& inputCommentLineIdentifiers,
          const std::string& inputCommentBlockOpenIdentifiers, const std::string& inputCommentBlockCloseIdentifiers);
    void advance();
    void expect(TokenType);
    Token getLookahead();
};