// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include "lexer.h"

// CommentChars, built from user input
std::string LineCommentChars;
std::string BlockCommentCharsOpen;
std::string BlockCommentCharsClose;
std::string PayloadCommentChars;
unsigned int MaxLookahead;

// Input data to be worked with
std::string FileInput;
int InputStringPos = 0;
int CurrentChar = 0;
unsigned int LineNum = 1;
unsigned int ColNum = 0;

Context currentContext = ARBITRARY; // Default ARBITRARY

int advance() {
    // Return EOF, when string ends
    if (InputStringPos >= FileInput.length() - 1)
        return CurrentChar = EOF;
    InputStringPos++;
    ColNum++;
    return CurrentChar = (unsigned char) FileInput[InputStringPos];
}

void expect(int input) {
    if (CurrentChar != input)
        throw std::runtime_error("Expected '" + std::string(1, (char) input) + "', but got '"
                                 + std::string(1, (char) CurrentChar) +"' at L"
                                 + std::to_string(LineNum) + "C" + std::to_string(ColNum));
    advance();
}

std::string getLookahead() {
    int length = std::min((int) FileInput.length() - InputStringPos, (int) MaxLookahead);
    return FileInput.substr(InputStringPos, length);
}

Token getTok() {
    // Skip any whitespace
    skipWhitespaces();

    // Check for EOF
    if (isEOF()) return Token(TOK_EOF, LineNum, ColNum);

    // Are we in arbitrary context?
    if (currentContext == ARBITRARY) {
        Token token = consumeArbitrary();
        if (!token.getValue().empty())
            return token;
    }

    // Are we in payload context?
    if (currentContext == PAYLOAD) return consumePayload();

    // Is it a primitive char, that can be returned immediately?
    switch (CurrentChar) {
        case '.': {
            expect('.');
            return Token(TOK_DOT, LineNum, ColNum);
        }
        case '{': {
            expect('{');
            currentContext = PAYLOAD;
            return Token(TOK_BRACE_OPEN, LineNum, ColNum);
        }
        case '}': {
            expect('}');
            if (!isLookaheadBlockCommentCharClose())
                currentContext = ARBITRARY;
            return Token(TOK_BRACE_CLOSE, LineNum, ColNum);
        }
        case '[': {
            expect('[');
            std::string idx = consumeNumber();
            expect(']');
            return Token(TOK_INDEX, idx, LineNum, ColNum);
        }
        case '|': {
            expect('|');
            return Token(TOK_OR, LineNum, ColNum);
        }
        case '!': { // !=
            expect('!');
            expect('=');
            return Token(TOK_NOT_EQUALS, LineNum, ColNum);
        }
        case '=': { // ==
            expect('=');
            expect('=');
            return Token(TOK_EQUALS, LineNum, ColNum);
        }
        case '"': { // String literal
            return consumeStringLiteral();
        }
        default: {
            // Is it a char array?
            if (isalpha(CurrentChar)) return consumeIdentifierOrKeyword();

            // Is it an integer?
            if (isdigit(CurrentChar)) return Token(TOK_NUMBER, consumeNumber(), LineNum, ColNum);

            // Is it a conditional comment identifier?
            std::string laResult;
            if (isLookaheadLineCommentChars()) {
                // Consume LineCommentChars
                for (int i = 0; i < LineCommentChars.length(); i++) advance();
                // Update context
                currentContext = SECTION;
                return Token(TOK_COM_LINE_IDEN, LineNum, ColNum);
            } else if (isLookaheadPayloadCommentChars()) {
                // Consume PayloadCommentChars
                for (int i = 0; i < PayloadCommentChars.length(); i++) advance();
                return Token(TOK_COM_IDEN_PAYLOAD, LineNum, ColNum);
            } else if (isLookaheadBlockCommentCharOpen()) {
                // Consume BlockCommentCharsOpen
                for (int i = 0; i < BlockCommentCharsOpen.length(); i++) advance();
                // Update context
                currentContext = SECTION;
                return Token(TOK_COM_BLOCK_IDEN_OPEN, LineNum, ColNum);
            } else if (isLookaheadBlockCommentCharClose()) {
                // Consume BlockCommentCharsClose
                for (int i = 0; i < BlockCommentCharsClose.length(); i++) advance();
                // Update context
                currentContext = ARBITRARY;
                return Token(TOK_COM_BLOCK_IDEN_CLOSE, LineNum, ColNum);
            }
        }
    }

    // Otherwise, just return the character as its ascii value.
    Token result = Token(TOK_UNKNOWN, std::string(1, (char) CurrentChar), LineNum, ColNum);
    advance();
    return result;
}

bool isEOF() {
    return CurrentChar == EOF;
}

void skipWhitespaces() {
    while (isspace(CurrentChar)) {
        if (CurrentChar == '\n') {
            ColNum = -1;
            LineNum++;
        }
        advance();
    }
}

Token consumeArbitrary() {
    std::stringstream arbitraryStr;
    while(!isLookaheadLineCommentChars() && !isLookaheadBlockCommentCharOpen() && !isEOF()) {
        if (CurrentChar == '\n') {
            ColNum = -1;
            LineNum++;
        }
        arbitraryStr << (char) CurrentChar;
        advance();
    }
    currentContext = SECTION;
    return Token(TOK_ARBITRARY, arbitraryStr.str(), LineNum, ColNum);
}

Token consumePayload() {
    std::stringstream payloadStr;
    while(!isLookaheadLineCommentChars() && !isLookaheadBlockCommentCharCloseWithBrace() && !isEOF()) {
        if (CurrentChar == '\n') {
            ColNum = -1;
            LineNum++;
        } else if (isLookaheadPayloadCommentChars()) {
            // Consume PayloadCommentChars
            for (int i = 0; i < PayloadCommentChars.length(); i++) advance();
        }
        payloadStr << (char) CurrentChar;
        advance();
    }
    currentContext = SECTION;
    return Token(TOK_ARBITRARY, payloadStr.str(), LineNum, ColNum);
}

std::string consumeNumber() {
    std::stringstream numStr;
    do {
        numStr << (char) CurrentChar;
        advance();
    } while (isdigit(CurrentChar));
    return numStr.str();
}

Token consumeIdentifierOrKeyword() {
    std::stringstream identifierStr;
    identifierStr << (char) CurrentChar;
    while (isalnum(advance())) // [a-zA-Z0-9]*
        identifierStr << (char) CurrentChar;

    // Is keyword?
    std::string identifierName = identifierStr.str();
    if (identifierName == "if") return Token(TOK_IF, LineNum, ColNum);
    if (identifierName == "has") return Token(TOK_HAS, LineNum, ColNum);
    if (identifierName == "not") return Token(TOK_NOT, LineNum, ColNum);
    if (identifierName == "true") return Token(TOK_TRUE, LineNum, ColNum);
    if (identifierName == "false") return Token(TOK_FALSE, LineNum, ColNum);
    return Token(TOK_IDENTIFIER, identifierName, LineNum, ColNum);
}

Token consumeStringLiteral() {
    expect('"');
    std::stringstream stringStr;
    while(CurrentChar != '"' && CurrentChar != EOF) {
        if (CurrentChar == '\\') {
            stringStr << (char) advance();
            advance();
        } else {
            stringStr << (char) CurrentChar;
            advance();
        }
    }
    expect('"');
    return Token(TOK_STRING, stringStr.str(), LineNum, ColNum);
}

bool isLookaheadPayloadCommentChars() {
    return !PayloadCommentChars.empty()
        && getLookahead().substr(0, PayloadCommentChars.length()) == PayloadCommentChars;
}

bool isLookaheadLineCommentChars() {
    return !LineCommentChars.empty()
        && getLookahead().substr(0, LineCommentChars.length()) == LineCommentChars;
}

bool isLookaheadBlockCommentCharOpen() {
    return !BlockCommentCharsOpen.empty()
        && getLookahead().substr(0, BlockCommentCharsOpen.length()) == BlockCommentCharsOpen;
}

bool isLookaheadBlockCommentCharClose() {
    return !BlockCommentCharsClose.empty()
        && getLookahead().substr(0, BlockCommentCharsClose.length()) == BlockCommentCharsClose;
}

bool isLookaheadBlockCommentCharCloseWithBrace() {
    return !BlockCommentCharsClose.empty()
           && getLookahead().substr(0, BlockCommentCharsClose.length() +1) == "}" + BlockCommentCharsClose;
}

void initLexer(bool isSingleStatement, const std::string& fileInput, const std::string& lineCommentChars,
               const std::string& blockCommentCharsOpen, const std::string& blockCommentCharsClose) {
    FileInput = fileInput;

    // Build conditional comment chars, based on comment chars input
    LineCommentChars = lineCommentChars.empty() ? "" : lineCommentChars + "?";
    BlockCommentCharsOpen = blockCommentCharsOpen.empty() ? "" : blockCommentCharsOpen + "?";
    BlockCommentCharsClose = blockCommentCharsClose;
    PayloadCommentChars = lineCommentChars;
    MaxLookahead = std::max({LineCommentChars.length(), BlockCommentCharsOpen.length(),
                             BlockCommentCharsClose.length(), PayloadCommentChars.length()}) +1;
    currentContext = isSingleStatement ? SECTION : ARBITRARY;

    // Load first char into the buffer
    advance();
}
