// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include "parser.h"

// Token buffer
Token CurTok;
Token getNextToken() { return CurTok = getTok(); }

std::unique_ptr<ExprAST> parseNumber() {
    std::string stringValue = CurTok.getValue();
    auto result = std::make_unique<NumberExprAST>(stoi(stringValue));
    getNextToken(); // Consume number literal
    return std::move(result);
}

std::unique_ptr<ExprAST> parseString() {
    std::string value = CurTok.getValue();
    auto result = std::make_unique<StringExprAST>(value);
    getNextToken();
    return std::move(result);
}

std::unique_ptr<ExprAST> parseValue() {

}

std::unique_ptr<ExprAST> parseIdentifierExpr() {
    std::string value = CurTok.getValue();
    auto result = std::make_unique<IdentifierExprAST>(value);
    getNextToken();
    return std::move(result);
}

std::unique_ptr<ExprAST> parseKey() {

}

std::unique_ptr<ExprAST> parseCompStmt() {

}

std::unique_ptr<ExprAST> parseHasStmt() {

}

std::unique_ptr<ExprAST> parseStmt() {

}

std::unique_ptr<ExprAST> parseStmtList() {

}

std::unique_ptr<ExprAST> parsePayload() {

}

std::unique_ptr<ExprAST> parseComBlockIdenClose() {

}

std::unique_ptr<ExprAST> parseComBlockIdenOpen() {

}

std::unique_ptr<ExprAST> parseComIdenPayload() {

}

std::unique_ptr<ExprAST> parseComLineIden() {

}

std::unique_ptr<ExprAST> parseIfBlock() {

}

std::unique_ptr<ExprAST> parseComBlockBlock() {

}

std::unique_ptr<ExprAST> parseComLineBlock() {

}

std::unique_ptr<ExprAST> parseSection() {

}

std::unique_ptr<ExprAST> parseContent() {

}

void initParser(const bool isSingleStatement, const std::string& fileInput, const std::string& dataInput,
                const std::string& lineCommentChars, const std::string& blockCommentCharsOpen,
                const std::string& blockCommentCharsClose) {
    initLexer(fileInput, lineCommentChars, blockCommentCharsOpen, blockCommentCharsClose);

    // Test lexer
    Token next;
    while ((next = getNextToken()).getType() != TOK_EOF) {
        std::cout << "Got token: " << std::to_string(next.getType()) << std::endl;
        std::cout << "Value: " << next.getValue() << std::endl;
        std::cout << "Loc: " << next.getCodePos() << std::endl;
        std::cout << std::endl;
    }
}