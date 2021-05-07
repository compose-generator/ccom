// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include <iostream>
#include "lexer.h"

// CommentChars, built from user input
std::string LineCommentChars;
std::string BlockCommentCharsOpen;
std::string BlockCommentCharsClose;
std::string PayloadCommentChars;
unsigned int MaxLookahead;

// Input data to be worked with
std::string FileInput;
int InputStringPos = -1;
int CurrentChar = 0;
unsigned int LineNum = 1;
unsigned int ColNum = 0;

Context currentContext = ARBITRARY;


int advance() {
    InputStringPos++;
    if (InputStringPos > FileInput.length() - 1) {
        CurrentChar = EOF; // Return EOF, when string ends
    } else {
        CurrentChar = (unsigned char) FileInput[InputStringPos];
        ColNum++;
    }
    return CurrentChar;
}

int expect(int input) {
    if (CurrentChar != input)
        throw std::runtime_error("Expected '" + std::string(1, (char) input) + "', but got '"
                                 + std::string(1, (char) CurrentChar) +"' at L"
                                 + std::to_string(LineNum) + "C" + std::to_string(ColNum));
    advance();
    return input;
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
        return consumeArbitrary();
    }

    // Are we in payload context?
    if (currentContext == PAYLOAD) return consumePayload();

    // Is it a primitive char, that can be returned immediately?
    switch (CurrentChar) {
        case '.':
            expect('.');
            return Token(TOK_DOT, LineNum, ColNum);
        case '{':
            expect('{');
            currentContext = PAYLOAD;
            return Token(TOK_BRACE_OPEN, LineNum, ColNum);
        case '}':
            expect('}');
            currentContext = ARBITRARY;
            return Token(TOK_BRACE_CLOSE, LineNum, ColNum);
        case '|':
            expect('|');
            return Token(TOK_OR, LineNum, ColNum);
        case '!': // !=
            expect('!');
            expect('=');
            return Token(TOK_NOT_EQUALS, LineNum, ColNum);
        case '=': // ==
            expect('=');
            expect('=');
            return Token(TOK_EQUALS, LineNum, ColNum);
        case '"': // String literal
            return consumeStringLiteral();
        default:
            // Is it a char array?
            if (isalpha(CurrentChar)) return consumeIdentifierOrKeyword();

            // Is it an integer?
            if (isdigit(CurrentChar)) return consumeNumber();

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
    std::string arbitraryStr;
    while(!isLookaheadLineCommentChars() && !isLookaheadBlockCommentCharOpen() && !isEOF()) {
        if (CurrentChar == '\n') {
            ColNum = -1;
            LineNum++;
        }
        arbitraryStr.push_back((char) CurrentChar);
        advance();
    }
    currentContext = SECTION;
    return Token(TOK_ARBITRARY, arbitraryStr, LineNum, ColNum);
}

Token consumePayload() {
    std::string payloadStr;
    while(!isLookaheadLineCommentChars() && !isLookaheadBlockCommentCharClose() && !isEOF()) {
        if (CurrentChar == '\n') {
            ColNum = -1;
            LineNum++;
        } else if (isLookaheadPayloadCommentChars()) {
            // Consume PayloadCommentChars
            for (int i = 0; i < PayloadCommentChars.length(); i++) advance();
        }
        payloadStr.push_back((char) CurrentChar);
        advance();
    }
    currentContext = SECTION;
    return Token(TOK_ARBITRARY, payloadStr, LineNum, ColNum);
}

Token consumeNumber() {
    std::string numStr;
    do {
        numStr.push_back((char) CurrentChar);
        advance();
    } while (isdigit(CurrentChar));
    return Token(TOK_NUMBER, numStr, LineNum, ColNum);
}

Token consumeIdentifierOrKeyword() {
    std::string identifierStr;
    identifierStr.push_back((char) CurrentChar);
    while (isalnum(advance())) // [a-zA-Z0-9]*
        identifierStr.push_back((char) CurrentChar);

    // Is keyword?
    if (identifierStr == "if") return Token(TOK_IF, LineNum, ColNum);
    if (identifierStr == "has") return Token(TOK_HAS, LineNum, ColNum);
    if (identifierStr == "not") return Token(TOK_NOT, LineNum, ColNum);
    if (identifierStr == "true") return Token(TOK_TRUE, LineNum, ColNum);
    if (identifierStr == "false") return Token(TOK_FALSE, LineNum, ColNum);
    return Token(TOK_IDENTIFIER, identifierStr, LineNum, ColNum);
}

Token consumeStringLiteral() {
    expect('"');
    std::string stringStr;
    while(CurrentChar != '"' && CurrentChar != EOF) {
        if (CurrentChar == '\\') {
            stringStr.push_back((char) advance());
            advance();
        } else {
            stringStr.push_back((char) CurrentChar);
            advance();
        }
    }
    expect('"');
    return Token(TOK_STRING, stringStr, LineNum, ColNum);
}

bool isLookaheadPayloadCommentChars() {
    return getLookahead().substr(0, PayloadCommentChars.length()) == PayloadCommentChars;
}

bool isLookaheadLineCommentChars() {
    return !LineCommentChars.empty() && getLookahead().substr(0, LineCommentChars.length()) == LineCommentChars;
}

bool isLookaheadBlockCommentCharOpen() {
    return !BlockCommentCharsOpen.empty() && !BlockCommentCharsClose.empty()
        && getLookahead().substr(0, BlockCommentCharsOpen.length()) == BlockCommentCharsOpen;
}

bool isLookaheadBlockCommentCharClose() {
    return !BlockCommentCharsOpen.empty() && !BlockCommentCharsClose.empty()
        && getLookahead().substr(0, BlockCommentCharsClose.length()) == BlockCommentCharsClose;
}

void initLexer(const std::string& fileInput, const std::string& lineCommentChars,
               const std::string& blockCommentCharsOpen, const std::string& blockCommentCharsClose) {
    FileInput = fileInput;

    // Build conditional comment chars, based on comment chars input
    LineCommentChars = lineCommentChars.empty() ? "" : lineCommentChars + "?";
    BlockCommentCharsOpen = blockCommentCharsOpen.empty() ? "" : blockCommentCharsOpen + "?";
    BlockCommentCharsClose = blockCommentCharsClose;
    PayloadCommentChars = lineCommentChars;
    MaxLookahead = std::max({LineCommentChars.length(), BlockCommentCharsOpen.length(),
                             BlockCommentCharsClose.length(), PayloadCommentChars.length()});

    //std::cout << "LineCommentChars: " << LineCommentChars << std::endl;
    //std::cout << "BlockCommentCharsOpen: " << BlockCommentCharsOpen << std::endl;
    //std::cout << "BlockCommentCharsClose: " << BlockCommentCharsClose << std::endl;
    //std::cout << "PayloadCommentChars: " << PayloadCommentChars << std::endl;

    // Load first char into the buffer
    advance();
}
