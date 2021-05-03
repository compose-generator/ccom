//
// Created by Marc on 02.05.2021.
//

#include <string>
#include <stdexcept>
#include <algorithm>
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
    while (isspace(CurrentChar)) {
        if (CurrentChar == '\n') {
            ColNum = -1;
            LineNum++;
        }
        advance();
    }

    // Check for EOF
    if (CurrentChar == EOF) return Token(TOK_EOF, LineNum, ColNum);

    // Is it a char array?
    if (isalpha(CurrentChar)) { // [a-zA-Z]
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

    // Is it an integer?
    if (isdigit(CurrentChar)) { // [0-9]+
        std::string NumStr;
        do {
            NumStr += std::to_string(CurrentChar);
            advance();
        } while (isdigit(CurrentChar));
        return Token(TOK_NUMBER, NumStr, LineNum, ColNum);
    }

    // Is it a single char, that can be returned immediately?
    if (CurrentChar == '.') {
        advance();
        return Token(TOK_DOT, LineNum, ColNum);
    }
    if (CurrentChar == '{') {
        advance();
        return Token(TOK_BRACE_OPEN, LineNum, ColNum);
    }
    if (CurrentChar == '}') {
        advance();
        return Token(TOK_BRACE_CLOSE, LineNum, ColNum);
    }
    if (CurrentChar == '|') {
        advance();
        return Token(TOK_OR, LineNum, ColNum);
    }

    // Is it '!='?
    if (CurrentChar == '!') {
        expect('!');
        expect('=');
        return Token(TOK_NOT_EQUALS, LineNum, ColNum);
    }

    // Is it '=='?
    if (CurrentChar == '=') {
        expect('=');
        expect('=');
        return Token(TOK_EQUALS, LineNum, ColNum);
    }

    // Is it a string literal?
    if (CurrentChar == '"') {
        expect('"');
        std::string stringStr;
        stringStr.push_back((char) CurrentChar);
        while(CurrentChar != '"' && CurrentChar != EOF) {
            stringStr.push_back((char) advance());
            if (CurrentChar == '\\') {
                advance();
                stringStr.push_back((char) advance());
            }
        }
        expect('"');
        return Token(TOK_STRING, LineNum, ColNum);
    }

    // Is it a conditional comment identifier?
    std::string laResult;
    if ((laResult = getLookahead().substr(0, PayloadCommentChars.length())) == PayloadCommentChars) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return Token(TOK_COM_IDEN_PAYLOAD, LineNum, ColNum);
    }
    if ((laResult = getLookahead().substr(0, LineCommentChars.length())) == LineCommentChars) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return Token(TOK_COM_LINE_IDEN, LineNum, ColNum);
    }
    if ((laResult = getLookahead().substr(0, BlockCommentCharsOpen.length())) == BlockCommentCharsOpen) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return Token(TOK_COM_BLOCK_IDEN_OPEN, LineNum, ColNum);
    }
    if ((laResult = getLookahead().substr(0, BlockCommentCharsClose.length())) == BlockCommentCharsClose) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return Token(TOK_COM_BLOCK_IDEN_CLOSE, LineNum, ColNum);
    }

    // Otherwise, just return the character as its ascii value.
    Token result = Token(TOK_UNKNOWN, std::string(1, (char) CurrentChar), LineNum, ColNum);
    advance();
    return result;
}

void initLexer(std::string fileInput,
               const std::string& lineCommentChars,
               const std::string& blockCommentCharsOpen,
               std::string blockCommentCharsClose) {
    FileInput = std::move(fileInput);

    // Build conditional comment chars, based on comment chars input
    LineCommentChars = lineCommentChars + "?";
    BlockCommentCharsOpen = blockCommentCharsOpen + "?";
    BlockCommentCharsClose = std::move(blockCommentCharsClose);
    PayloadCommentChars = LineCommentChars + "?";
    MaxLookahead = std::max({BlockCommentCharsOpen.length(), BlockCommentCharsClose.length(), PayloadCommentChars.length()});

    // Load first char into the buffer
    advance();
}
