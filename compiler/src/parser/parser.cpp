//
// Created by Marc on 02.05.2021.
//

#include <string>
#include <iostream>
#include "parser.h"
#include "../lexer/lexer.h"

// Token buffer
Token CurTok;
Token getNextToken() { return CurTok = getTok(); }

void initParser(std::string fileInput,
                std::string dataInput,
                const std::string& lineCommentChars,
                const std::string& blockCommentCharsOpen,
                std::string blockCommentCharsClose) {
    initLexer(std::move(fileInput), lineCommentChars, blockCommentCharsOpen, std::move(blockCommentCharsClose));

    // Test lexer
    Token next;
    while ((next = getNextToken()).getType() != TOK_EOF) {
        std::cout << "Got token: " << std::to_string(next.getType()) << std::endl;
    }
}