#include <string>
#include <cstdio>

enum Token {
    tok_eof                     = -1,

    // keywords
    tok_if                      = -2,
    tok_has                     = -3,
    tok_not                     = -4,

    // primary
    tok_identifier              = -5,
    tok_number                  = -6,
    tok_string                  = -7,
    tok_com_line_iden           = -8,
    tok_com_iden_payload        = -9,
    tok_com_block_iden_open     = -10,
    tok_com_block_iden_close    = -11,
};

static std::string IdentifierStr;
static double NumVal;

static int getTok() {
    static int LastChar = ' ';

    // Skip any whitespace
    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar)) { // [a-zA-Z][a-zA-Z0-9]*
        IdentifierStr = std::to_string(LastChar);
        while (isalnum(LastChar = getchar()))
            IdentifierStr += std::to_string(LastChar);

        // Is keyword?
        if (IdentifierStr == "if")
            return tok_if;
        if (IdentifierStr == "has")
            return tok_has;
        if (IdentifierStr == "not")
            return tok_not;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') { // [0-9.]+
        std::string NumStr;
        do {
            NumStr += std::to_string(LastChar);
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    // Check for EOF
    if (LastChar == EOF)
        return tok_eof;

    // Otherwise, return char as its ascii value
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

int main() {
    // Prompt user for input
    fprintf(stderr, "ready> ");

    int token = getTok();

    return 0;
}