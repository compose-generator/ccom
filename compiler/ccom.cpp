#include <string>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>

enum Token {
    // End of file
    TOK_EOF                         = -1,

    // Keywords
    TOK_IF                          = -2, // if
    TOK_HAS                         = -3, // has
    TOK_NOT                         = -4, // not

    // Boolean values
    TOK_TRUE                        = -5, // true
    TOK_FALSE                       = -6, // false

    // Operators
    TOK_OR                          = -7, // |
    TOK_EQUALS                      = -8, // ==
    TOK_NOT_EQUALS                  = -9, // !=

    // Misc
    TOK_IDENTIFIER                  = -10, // e.g. test
    TOK_NUMBER                      = -11, // e.g. 123
    TOK_STRING                      = -12, // "test"
    TOK_DOT                         = -13, // .
    TOK_BRACE_OPEN                  = -14, // {
    TOK_BRACE_CLOSE                 = -15, // }
    TOK_COM_LINE_IDEN               = -16, // //!
    TOK_COM_BLOCK_IDEN_OPEN         = -17, // /*!
    TOK_COM_BLOCK_IDEN_CLOSE        = -18, // */
    TOK_COM_IDEN_PAYLOAD            = -19, // //!!
};

static std::string FileInput;
static int InputStringPos = -1;
static int CurrentChar = 0;

static std::string LineCommentChars;
static std::string BlockCommentCharsOpen;
static std::string BlockCommentCharsClose;
static std::string PayloadCommentChars;

static std::string IdentifierStr;
static int NumVal;

// ------------------------------------------------------- Lexer -------------------------------------------------------

static int advance() {
    InputStringPos++;
    if (InputStringPos > FileInput.length() - 1) {
        CurrentChar = EOF; // Return EOF, when string ends
    } else {
        CurrentChar = (unsigned char) FileInput[InputStringPos];
    }
    return CurrentChar;
}

static int expect(int input) {
    if (CurrentChar != input)
        throw std::runtime_error("Expected '" + std::string(1, (char) input) + "', but got '"
            + std::string(1, (char) CurrentChar) +"'");
    advance();
    return input;
}

static std::string variableLookahead(unsigned int length) {
    length = std::min((int)FileInput.length() - InputStringPos, (int)length);
    return FileInput.substr(InputStringPos, length);
}

static int getTok() {
    // Skip any whitespace
    while (isspace(CurrentChar)) advance();

    // Check for EOF
    if (CurrentChar == EOF) return TOK_EOF;

    // Is it a char array?
    if (isalpha(CurrentChar)) { // [a-zA-Z]
        IdentifierStr = (char) CurrentChar;
        while (isalnum(advance())) // [a-zA-Z0-9]*
            IdentifierStr.push_back((char) CurrentChar);

        // Is keyword?
        if (IdentifierStr == "if") return TOK_IF;
        if (IdentifierStr == "has") return TOK_HAS;
        if (IdentifierStr == "not") return TOK_NOT;
        if (IdentifierStr == "true") return TOK_TRUE;
        if (IdentifierStr == "false") return TOK_FALSE;
        return TOK_IDENTIFIER;
    }

    // Is it an integer?
    if (isdigit(CurrentChar)) { // [0-9]+
        std::string NumStr;
        do {
            NumStr += std::to_string(CurrentChar);
            advance();
        } while (isdigit(CurrentChar));
        NumVal = std::stoi(NumStr);
        return TOK_NUMBER;
    }

    // Is it a single char, that can be returned immediately?
    if (CurrentChar == '.') {
        advance();
        return TOK_DOT;
    }
    if (CurrentChar == '{') {
        advance();
        return TOK_BRACE_OPEN;
    }
    if (CurrentChar == '}') {
        advance();
        return TOK_BRACE_CLOSE;
    }
    if (CurrentChar == '|') {
        advance();
        return TOK_OR;
    }

    // Is it '!='?
    if (CurrentChar == '!') {
        expect('!');
        expect('=');
        return TOK_NOT_EQUALS;
    }

    // Is it '=='?
    if (CurrentChar == '=') {
        expect('=');
        expect('=');
        return TOK_EQUALS;
    }

    // Is it a string literal?
    if (CurrentChar == '"') {
        expect('"');
        while(CurrentChar != '"' && CurrentChar != EOF)
            CurrentChar = advance();
        expect('"');
        return TOK_STRING;
    }

    // Is it a conditional comment identifier?
    std::string laResult;
    //std::cout << "Lookahead: " << variableLookahead(4) << std::endl;
    if ((laResult = variableLookahead(PayloadCommentChars.length())) == PayloadCommentChars) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return TOK_COM_IDEN_PAYLOAD;
    }
    if ((laResult = variableLookahead(LineCommentChars.length())) == LineCommentChars) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return TOK_COM_LINE_IDEN;
    }
    if ((laResult = variableLookahead(BlockCommentCharsOpen.length())) == BlockCommentCharsOpen) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return TOK_COM_BLOCK_IDEN_OPEN;
    }
    if ((laResult = variableLookahead(BlockCommentCharsClose.length())) == BlockCommentCharsClose) {
        for (int i = 0; i < laResult.length(); i++) advance();
        return TOK_COM_BLOCK_IDEN_CLOSE;
    }

    // Otherwise, just return the character as its ascii value.
    int ThisChar = CurrentChar;
    advance();
    return ThisChar;
}

// ------------------------------------------------------- Parser ------------------------------------------------------

// Token buffer
static int CurTok;
static int getNextToken() { return CurTok = getTok(); }



// ---------------------------------------------------- Main program ---------------------------------------------------

// CLI call: ./ccom <file-input> <data-input> <line-com-chars> <block-com-open-chars> <block-com-close-chars>
int main(int argc, char** argv) {
    // Parse cli args
    std::vector<std::string> args;
    for (size_t iArg = 0; iArg < argc; ++iArg)
        args.emplace_back(argv[iArg]);

    FileInput = args[1];
    std::string dataInput = args[2];

    // Build conditional comment chars, based on comment chars input
    LineCommentChars = args[3] + "!";
    BlockCommentCharsOpen = args[4] + "!";
    BlockCommentCharsClose = args[5];
    PayloadCommentChars = LineCommentChars + "!";

    // Replace file input with test string
    FileInput = "property1: value\n//! if has frontend | has service.backend | var.FlaskPort == 8080 {\n//!! test payload\n//! }\nattribute2: value2";

    // Load first char into the buffer
    advance();

    // Test lexer
    int next;
    while ((next = getNextToken()) != TOK_EOF) {
        std::cout << "Got token: " << std::to_string(next) << std::endl;
    }

    return 0;
}