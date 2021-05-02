//
// Created by Marc on 02.05.2021.
//

#include <string>
#include <utility>
#include <stdexcept>
#include "lexer.h"
#include "Token.h"

// CommentChars, built from user input
std::string LineCommentChars;
std::string BlockCommentCharsOpen;
std::string BlockCommentCharsClose;
std::string PayloadCommentChars;

// Input data to be worked with
std::string FileInput;
int InputStringPos = -1;
int CurrentChar = 0;

int advance() {
    InputStringPos++;
    if (InputStringPos > FileInput.length() - 1) {
        CurrentChar = EOF; // Return EOF, when string ends
    } else {
        CurrentChar = (unsigned char) FileInput[InputStringPos];
    }
    return CurrentChar;
}

int expect(int input) {
    if (CurrentChar != input)
        throw std::runtime_error("Expected '" + std::string(1, (char) input) + "', but got '"
                                 + std::string(1, (char) CurrentChar) +"'");
    advance();
    return input;
}

std::string variableLookahead(unsigned int length) {
    length = std::min((int)FileInput.length() - InputStringPos, (int)length);
    return FileInput.substr(InputStringPos, length);
}

Token getTok() {
    // Skip any whitespace
    while (isspace(CurrentChar)) advance();

    // Check for EOF
    if (CurrentChar == EOF) return Token(TOK_EOF);

    // Is it a char array?
    if (isalpha(CurrentChar)) { // [a-zA-Z]
        std::string identifierStr;
        identifierStr.push_back((char) CurrentChar);
        while (isalnum(advance())) // [a-zA-Z0-9]*
            identifierStr.push_back((char) CurrentChar);

        // Is keyword?
        if (identifierStr == "if") return Token(TOK_IF);
        if (identifierStr == "has") return Token(TOK_HAS);
        if (identifierStr == "not") return Token(TOK_NOT);
        if (identifierStr == "true") return Token(TOK_TRUE);
        if (identifierStr == "false") return Token(TOK_FALSE);
        return Token(TOK_IDENTIFIER, identifierStr);
    }

    // Is it an integer?
    if (isdigit(CurrentChar)) { // [0-9]+
        std::string NumStr;
        do {
            NumStr += std::to_string(CurrentChar);
            advance();
        } while (isdigit(CurrentChar));
        return Token(TOK_NUMBER, std::stoi(NumStr));
    }

    // Is it a single char, that can be returned immediately?
    if (CurrentChar == '.') {
        advance();
        return Token(TOK_DOT);
    }
    if (CurrentChar == '{') {
        advance();
        return Token(TOK_BRACE_OPEN);
    }
    if (CurrentChar == '}') {
        advance();
        return Token(TOK_BRACE_CLOSE);
    }
    if (CurrentChar == '|') {
        advance();
        return Token(TOK_OR);
    }

    // Is it '!='?
    if (CurrentChar == '!') {
        expect('!');
        expect('=');
        return Token(TOK_NOT_EQUALS);
    }

    // Is it '=='?
    if (CurrentChar == '=') {
        expect('=');
        expect('=');
        return Token(TOK_EQUALS);
    }

    // Is it a string literal?
    if (CurrentChar == '"') {
        expect('"');
        std::string stringStr;
        stringStr.push_back((char) CurrentChar);
        while(CurrentChar != '"' && CurrentChar != EOF)
            stringStr.push_back((char) advance());
        expect('"');
        return Token(TOK_STRING);
    }

    // Is it a conditional comment identifier?
    std::string laResult;
    if ((laResult = variableLookahead(PayloadCommentChars.length())) == PayloadCommentChars) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return Token(TOK_COM_IDEN_PAYLOAD);
    }
    if ((laResult = variableLookahead(LineCommentChars.length())) == LineCommentChars) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return Token(TOK_COM_LINE_IDEN);
    }
    if ((laResult = variableLookahead(BlockCommentCharsOpen.length())) == BlockCommentCharsOpen) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return Token(TOK_COM_BLOCK_IDEN_OPEN);
    }
    if ((laResult = variableLookahead(BlockCommentCharsClose.length())) == BlockCommentCharsClose) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return Token(TOK_COM_BLOCK_IDEN_CLOSE);
    }

    // Otherwise, just return the character as its ascii value.
    int ThisChar = CurrentChar;
    advance();
    return Token(TOK_UNKNOWN, ThisChar);
}

void initLexer(std::string fileInput,
               const std::string& lineCommentChars,
               const std::string& blockCommentCharsOpen,
               std::string blockCommentCharsClose) {
    FileInput = std::move(fileInput);

    // Build conditional comment chars, based on comment chars input
    LineCommentChars = lineCommentChars + "!";
    BlockCommentCharsOpen = blockCommentCharsOpen + "!";
    BlockCommentCharsClose = std::move(blockCommentCharsClose);
    PayloadCommentChars = LineCommentChars + "!";

    // Load first char into the buffer
    advance();
}
