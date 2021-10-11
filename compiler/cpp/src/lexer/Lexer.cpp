/*
Copyright © 2021 Compose Generator Contributors
All rights reserved.
*/

// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "Lexer.h"

Lexer::Lexer(bool isSingleStatement, const std::string &fileInput, const std::string &inputCommentLineIdentifiers,
             const std::string &inputCommentBlockOpenIdentifiers, const std::string &inputCommentBlockCloseIdentifiers) {
    // Build conditional comment chars, based on comment chars input
    commentLineIdentifier = inputCommentLineIdentifiers.empty() ? "" : inputCommentLineIdentifiers + "?";
    commentBlockOpenIdentifier = inputCommentBlockOpenIdentifiers.empty() ? "" : inputCommentBlockOpenIdentifiers + "?";
    commentBlockCloseIdentifier = inputCommentBlockCloseIdentifiers;
    commentPayloadIdentifier = inputCommentLineIdentifiers;

    // +1: See isLookAheadCommentBlockCloseIdentifierWithBrace method.
    // Max lookahead has to be max +1 because of the closed brace
    unsigned int maxLookahead = std::max({commentLineIdentifier.length(), commentBlockOpenIdentifier.length(),
        commentBlockCloseIdentifier.length(),commentPayloadIdentifier.length()}) + 1;

    // Initialize reader
    reader = Reader(fileInput, maxLookahead);

    currentContext = isSingleStatement || isLookAheadCommentLineIdentifier() ||
            isLookAheadCommentBlockOpenIdentifier() ? SECTION : ARBITRARY;

    advance(); // Load first token into buffer
}

void Lexer::advance() {
    // Skip any whitespaces
    while (currentContext != ARBITRARY && isspace(reader.getLookahead()))
        reader.advance();
    updateTokenStartPosition();

    // Avoid empty arbitrary at EOF
    if (isEOF()) {
        curTok = consumeEOF();
        return;
    }

    // Switch depending on context
    switch (currentContext) {
        case ARBITRARY:
            curTok = consumeArbitrary();
            return;
        case PAYLOAD:
            curTok = consumePayload();
            return;
        case SECTION:
            curTok = consumeSection();
            return;
    }
}

void Lexer::expect(TokenType expectedType) {
    if (curTok.getType() != expectedType)
        throw UnexpectedTokenException(expectedType, curTok.getType(), tokenStartLineNum, tokenStartColNum);
    advance();
}

Token Lexer::getLookahead() {
    return curTok;
}

// ------------------------------------------------ Private functions --------------------------------------------------

Token Lexer::consumeArbitrary() {
    std::stringstream arbitraryStr;
    while(!isLookAheadCommentLineIdentifier() && !isLookAheadCommentBlockOpenIdentifier() && !isEOF()) {
        arbitraryStr << (char) reader.getLookahead();
        reader.advance();
    }
    currentContext = SECTION;
    return constructToken(TOK_ARBITRARY, arbitraryStr.str());
}

Token Lexer::consumePayload() {
    std::stringstream payloadStr;
    while(!isLookAheadCommentLineIdentifier() && !isLookAheadCommentBlockCloseIdentifierWithBrace() && !isEOF()) {
        if (isLookAheadCommentPayloadIdentifier()) consumeCommentPayloadIdentifier();
        payloadStr << (char) reader.getLookahead();
        reader.advance();
    }
    currentContext = SECTION;
    return constructToken(TOK_ARBITRARY, payloadStr.str());
}

Token Lexer::consumeSection() {
    char curChar = (char) reader.getLookahead();

    if (isLookAheadCommentLineIdentifier()) return consumeCommentLineIdentifier();
    if (isLookAheadCommentBlockOpenIdentifier()) return consumeCommentBlockOpenIdentifier();
    if (isLookAheadCommentBlockCloseIdentifier()) return consumeCommentBlockCloseIdentifier();
    if (isLookAheadCommentPayloadIdentifier()) return consumeCommentPayloadIdentifier();

    switch (curChar) {
        case '|': return consumeOr();
        case '=': return consumeEquals();
        case '!': return consumeNotEquals();
        case '>': return consumeGreaterOrGreaterEqual();
        case '<': return consumeLessOrLessEqual();
        case '.': return consumeDot();
        case '{': return consumeBraceOpen();
        case '}': return consumeBraceClose();
        case '[': return consumeIndex();
        case '"': return consumeString();
    }

    if (isalpha(curChar) || curChar == '_') return consumeIdentifierOrKeyword();
    if (isdigit(curChar)) return consumeNumber();

    throw UnexpectedCharException(curChar, reader.getLineNum(), reader.getColNum());
}

Token Lexer::consumeEOF() {
    return constructToken(TOK_EOF);
}

Token Lexer::consumeOr() {
    reader.expect('|');
    return constructToken(TOK_OR);
}

Token Lexer::consumeEquals() {
    reader.expect('=');
    reader.expect('=');
    return constructToken(TOK_EQUALS);
}

Token Lexer::consumeNotEquals() {
    reader.expect('!');
    reader.expect('=');
    return constructToken(TOK_NOT_EQUALS);
}

Token Lexer::consumeGreaterOrGreaterEqual() {
    reader.expect('>');
    if (reader.getLookahead() == '=') {
        reader.expect('=');
        return constructToken(TOK_GREATER_EQUAL);
    }
    return constructToken(TOK_GREATER);
}

Token Lexer::consumeLessOrLessEqual() {
    reader.expect('<');
    if (reader.getLookahead() == '=') {
        reader.expect('=');
        return constructToken(TOK_LESS_EQUAL);
    }
    return constructToken(TOK_LESS);
}

Token Lexer::consumeDot() {
    reader.expect('.');
    return constructToken(TOK_DOT);
}

Token Lexer::consumeBraceOpen() {
    reader.expect('{');
    currentContext = PAYLOAD;
    return constructToken(TOK_BRACE_OPEN);
}

Token Lexer::consumeBraceClose() {
    reader.expect('}');
    if (!isLookAheadCommentBlockCloseIdentifier()) currentContext = ARBITRARY;
    return constructToken(TOK_BRACE_CLOSE);
}

Token Lexer::consumeIndex() {
    reader.expect('[');
    Token indexToken = consumeNumber();
    reader.expect(']');
    return constructToken(TOK_INDEX, indexToken.getValue());
}

Token Lexer::consumeString() {
    reader.expect('"');
    std::stringstream stringStr;
    while(reader.getLookahead() != '"' && reader.getLookahead() != EOF) {
        if (reader.getLookahead() == '\\') reader.advance();
        stringStr << (char) reader.getLookahead();
        reader.advance();
    }
    reader.expect('"');
    return constructToken(TOK_STRING, stringStr.str());
}

Token Lexer::consumeIdentifierOrKeyword() {
    std::stringstream identifierStr;
    identifierStr << (char) reader.getLookahead();
    reader.advance();
    while (isalnum(reader.getLookahead()) || reader.getLookahead() == '_') { // [a-zA-Z0-9_]*
        identifierStr << (char) reader.getLookahead();
        reader.advance();
    }

    // Is keyword?
    std::string identifierName = identifierStr.str();
    if (identifierName == "if") return constructToken(TOK_IF);
    if (identifierName == "has") return constructToken(TOK_HAS);
    if (identifierName == "not") return constructToken(TOK_NOT);
    if (identifierName == "true") return constructToken(TOK_TRUE);
    if (identifierName == "false") return constructToken(TOK_FALSE);
    if (identifierName == "contains") return constructToken(TOK_CONTAINS);

    return constructToken(TOK_IDENTIFIER, identifierName);
}

Token Lexer::consumeNumber() {
    std::stringstream numStr;
    do {
        numStr << (char) reader.getLookahead();
        reader.advance();
    } while (isdigit(reader.getLookahead()));
    return constructToken(TOK_NUMBER, numStr.str());
}

Token Lexer::consumeCommentLineIdentifier() {
    reader.expectMultiple(commentLineIdentifier);
    currentContext = SECTION;
    return constructToken(TOK_COM_LINE_IDEN);
}

Token Lexer::consumeCommentBlockOpenIdentifier() {
    reader.expectMultiple(commentBlockOpenIdentifier);
    currentContext = SECTION;
    return constructToken(TOK_COM_BLOCK_IDEN_OPEN);
}

Token Lexer::consumeCommentBlockCloseIdentifier() {
    reader.expectMultiple(commentBlockCloseIdentifier);
    currentContext = ARBITRARY;
    return constructToken(TOK_COM_BLOCK_IDEN_CLOSE);
}

Token Lexer::consumeCommentPayloadIdentifier() {
    reader.expectMultiple(commentPayloadIdentifier);
    return constructToken(TOK_COM_IDEN_PAYLOAD);
}

// -------------------------------------------------- Util functions ---------------------------------------------------

bool Lexer::isEOF() {
    return reader.getLookahead() == EOF;
}

bool Lexer::isLookAheadCommentLineIdentifier() {
    return !commentLineIdentifier.empty() && reader.getLookaheadMultiple().find(commentLineIdentifier) == 0;
}

bool Lexer::isLookAheadCommentBlockOpenIdentifier() {
    return !commentBlockOpenIdentifier.empty() && reader.getLookaheadMultiple().find(commentBlockOpenIdentifier) == 0;
}

bool Lexer::isLookAheadCommentBlockCloseIdentifier() {
    return !commentBlockCloseIdentifier.empty() && reader.getLookaheadMultiple().find(commentBlockCloseIdentifier) == 0;
}

bool Lexer::isLookAheadCommentPayloadIdentifier() {
    return !commentPayloadIdentifier.empty() && reader.getLookaheadMultiple().find(commentPayloadIdentifier) == 0;
}

bool Lexer::isLookAheadCommentBlockCloseIdentifierWithBrace() {
    return !commentBlockCloseIdentifier.empty() &&
            reader.getLookaheadMultiple().find("}" + commentBlockCloseIdentifier) == 0;
}

Token Lexer::constructToken(TokenType type) {
    return Token(type, tokenStartLineNum, tokenStartColNum);
}

Token Lexer::constructToken(TokenType type, std::string value) {
    return Token(type, std::move(value), tokenStartLineNum, tokenStartColNum);
}

void Lexer::updateTokenStartPosition() {
    tokenStartLineNum = reader.getLineNum();
    tokenStartColNum = reader.getColNum();
}
