// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include "lexer.h"

// CommentChars, built from user input
std::string lineCommentChars;
std::string blockCommentCharsOpen;
std::string blockCommentCharsClose;
std::string payloadCommentChars;
unsigned int maxLookahead;

// Input data to be worked with
std::string fileInput;
int inputStringPos = 0;
int curChar = 0;
unsigned int lineNum = 1;
unsigned int colNum = 0;

Context currentContext = ARBITRARY; // Default ARBITRARY

int advance() {
    // Return EOF, when string ends
    if (inputStringPos >= fileInput.length() - 1)
        return curChar = EOF;
    inputStringPos++;
    colNum++;
    return curChar = (unsigned char) fileInput[inputStringPos];
}

void expect(int input) {
    if (curChar != input)
        throw std::runtime_error("Expected '" + std::string(1, (char) input) + "', but got '"
                                 + std::string(1, (char) curChar) + "' at L"
                                 + std::to_string(lineNum) + "C" + std::to_string(colNum));
    advance();
}

std::string getLookahead() {
    int length = std::min((int) fileInput.length() - inputStringPos, (int) maxLookahead);
    return fileInput.substr(inputStringPos, length);
}

Token getTok() {
    // Skip any whitespace
    skipWhitespaces();

    // Check for EOF
    if (isEOF()) return Token(TOK_EOF, lineNum, colNum);

    // Are we in arbitrary context?
    if (currentContext == ARBITRARY) {
        Token token = consumeArbitrary();
        if (!token.getValue().empty())
            return token;
    }

    // Are we in payload context?
    if (currentContext == PAYLOAD) return consumePayload();

    // Is it a conditional comment identifier?
    std::string laResult;
    if (isLookaheadLineCommentChars()) {
        // Consume lineCommentChars
        for (int i = 0; i < lineCommentChars.length(); i++) advance();
        // Update context
        currentContext = SECTION;
        return Token(TOK_COM_LINE_IDEN, lineNum, colNum);
    } else if (isLookaheadPayloadCommentChars()) {
        // Consume payloadCommentChars
        for (int i = 0; i < payloadCommentChars.length(); i++) advance();
        return Token(TOK_COM_IDEN_PAYLOAD, lineNum, colNum);
    } else if (isLookaheadBlockCommentCharOpen()) {
        // Consume blockCommentCharsOpen
        for (int i = 0; i < blockCommentCharsOpen.length(); i++) advance();
        // Update context
        currentContext = SECTION;
        return Token(TOK_COM_BLOCK_IDEN_OPEN, lineNum, colNum);
    } else if (isLookaheadBlockCommentCharClose()) {
        // Consume blockCommentCharsClose
        for (int i = 0; i < blockCommentCharsClose.length(); i++) advance();
        // Update context
        currentContext = ARBITRARY;
        return Token(TOK_COM_BLOCK_IDEN_CLOSE, lineNum, colNum);
    }

    // Is it a primitive char, that can be returned immediately?
    switch (curChar) {
        case '.': {
            expect('.');
            return Token(TOK_DOT, lineNum, colNum);
        }
        case '{': {
            expect('{');
            currentContext = PAYLOAD;
            return Token(TOK_BRACE_OPEN, lineNum, colNum);
        }
        case '}': {
            expect('}');
            if (!isLookaheadBlockCommentCharClose())
                currentContext = ARBITRARY;
            return Token(TOK_BRACE_CLOSE, lineNum, colNum);
        }
        case '[': {
            expect('[');
            std::string idx = consumeNumber();
            expect(']');
            return Token(TOK_INDEX, idx, lineNum, colNum);
        }
        case '|': {
            expect('|');
            return Token(TOK_OR, lineNum, colNum);
        }
        case '!': { // !=
            expect('!');
            expect('=');
            return Token(TOK_NOT_EQUALS, lineNum, colNum);
        }
        case '=': { // ==
            expect('=');
            expect('=');
            return Token(TOK_EQUALS, lineNum, colNum);
        }
        case '<': { // < or <=
            expect('<');
            if (curChar == '=') {
                expect('=');
                return Token(TOK_LESS_EQUAL, lineNum, colNum);
            }
            return Token(TOK_LESS, lineNum, colNum);
        }
        case '>': { // > or >=
            expect('>');
            if (curChar == '=') {
                expect('=');
                return Token(TOK_GREATER_EQUAL, lineNum, colNum);
            }
            return Token(TOK_GREATER, lineNum, colNum);
        }
        case '"': { // String literal
            return consumeStringLiteral();
        }
        default: {
            // Is it a char array?
            if (isalpha(curChar)) return consumeIdentifierOrKeyword();

            // Is it an integer?
            if (isdigit(curChar)) return Token(TOK_NUMBER, consumeNumber(), lineNum, colNum);
        }
    }

    // Otherwise, just return the character as its ascii value.
    Token result = Token(TOK_UNKNOWN, std::string(1, (char) curChar), lineNum, colNum);
    advance();
    return result;
}

bool isEOF() {
    return curChar == EOF;
}

void skipWhitespaces() {
    while (isspace(curChar)) {
        if (curChar == '\n') {
            colNum = -1;
            lineNum++;
        }
        advance();
    }
}

Token consumeArbitrary() {
    std::stringstream arbitraryStr;
    while(!isLookaheadLineCommentChars() && !isLookaheadBlockCommentCharOpen() && !isEOF()) {
        if (curChar == '\n') {
            colNum = -1;
            lineNum++;
        }
        arbitraryStr << (char) curChar;
        advance();
    }
    currentContext = SECTION;
    return Token(TOK_ARBITRARY, arbitraryStr.str(), lineNum, colNum);
}

Token consumePayload() {
    std::stringstream payloadStr;
    while(!isLookaheadLineCommentChars() && !isLookaheadBlockCommentCharCloseWithBrace() && !isEOF()) {
        if (curChar == '\n') {
            colNum = -1;
            lineNum++;
        } else if (isLookaheadPayloadCommentChars()) {
            // Consume payloadCommentChars
            for (int i = 0; i < payloadCommentChars.length(); i++) advance();
        }
        payloadStr << (char) curChar;
        advance();
    }
    currentContext = SECTION;
    return Token(TOK_ARBITRARY, payloadStr.str(), lineNum, colNum);
}

std::string consumeNumber() {
    std::stringstream numStr;
    do {
        numStr << (char) curChar;
        advance();
    } while (isdigit(curChar));
    return numStr.str();
}

Token consumeIdentifierOrKeyword() {
    std::stringstream identifierStr;
    identifierStr << (char) curChar;
    while (isalnum(advance())) // [a-zA-Z0-9]*
        identifierStr << (char) curChar;

    // Is keyword?
    std::string identifierName = identifierStr.str();
    if (identifierName == "if") return Token(TOK_IF, lineNum, colNum);
    if (identifierName == "has") return Token(TOK_HAS, lineNum, colNum);
    if (identifierName == "not") return Token(TOK_NOT, lineNum, colNum);
    if (identifierName == "true") return Token(TOK_TRUE, lineNum, colNum);
    if (identifierName == "false") return Token(TOK_FALSE, lineNum, colNum);
    return Token(TOK_IDENTIFIER, identifierName, lineNum, colNum);
}

Token consumeStringLiteral() {
    expect('"');
    std::stringstream stringStr;
    while(curChar != '"' && curChar != EOF) {
        if (curChar == '\\') advance();
        stringStr << (char) curChar;
        advance();
    }
    expect('"');
    return Token(TOK_STRING, stringStr.str(), lineNum, colNum);
}

bool isLookaheadPayloadCommentChars() {
    return !payloadCommentChars.empty() && getLookahead().find(payloadCommentChars) == 0;
}

bool isLookaheadLineCommentChars() {
    return !lineCommentChars.empty() && getLookahead().find(lineCommentChars) == 0;
}

bool isLookaheadBlockCommentCharOpen() {
    return !blockCommentCharsOpen.empty() && getLookahead().find(blockCommentCharsOpen) == 0;
}

bool isLookaheadBlockCommentCharClose() {
    return !blockCommentCharsClose.empty() && getLookahead().find(blockCommentCharsClose) == 0;
}

bool isLookaheadBlockCommentCharCloseWithBrace() {
    return !blockCommentCharsClose.empty()
           && getLookahead().substr(0, blockCommentCharsClose.length() + 1) == "}" + blockCommentCharsClose;
}

void initLexer(bool isSingleStatement, const std::string& inputFileInput, const std::string& inputLineCommentChars,
               const std::string& inputBlockCommentCharsOpen, const std::string& inputBlockCommentCharsClose) {
    fileInput = inputFileInput;

    // Build conditional comment chars, based on comment chars input
    lineCommentChars = inputLineCommentChars.empty() ? "" : inputLineCommentChars + "?";
    blockCommentCharsOpen = inputBlockCommentCharsOpen.empty() ? "" : inputBlockCommentCharsOpen + "?";
    blockCommentCharsClose = inputBlockCommentCharsClose;
    payloadCommentChars = inputLineCommentChars;
    maxLookahead = std::max({lineCommentChars.length(), blockCommentCharsOpen.length(),
                             blockCommentCharsClose.length(), payloadCommentChars.length()});
    currentContext = isSingleStatement ? SECTION : ARBITRARY;

    // Load first char into the buffer
    advance();
}
