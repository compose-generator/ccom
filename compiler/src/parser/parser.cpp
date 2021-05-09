// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include "parser.h"

// Token buffer
Token CurTok;
Token getNextToken() { return CurTok = getTok(); }

std::unique_ptr<NumberExprAST> parseNumber() {
    getNextToken(); // Consume number literal
    return std::make_unique<NumberExprAST>(stoi(CurTok.getValue()));
}

std::unique_ptr<StringExprAST> parseString() {
    getNextToken();
    return std::make_unique<StringExprAST>(CurTok.getValue());
}

std::unique_ptr<ExprAST> parseValue() {
    if (CurTok.getType() == TOK_NUMBER) {

    } else {

    }
}

std::unique_ptr<IdentifierExprAST> parseIdentifier() {
    getNextToken(); // Consume identifier
    return std::make_unique<IdentifierExprAST>(CurTok.getValue());
}

std::unique_ptr<KeyExprAST> parseKey() {
    std::vector<std::unique_ptr<IdentifierExprAST>> identifiers;
    do {

    } while (CurTok.getType() == TOK_DOT);
    return std::make_unique<KeyExprAST>(identifiers);
}

std::unique_ptr<ExprAST> parseCompStmt() {

}

std::unique_ptr<ExprAST> parseHasStmt() {

}

std::unique_ptr<StmtExprAST> parseStmt() {

}

std::unique_ptr<StmtLstExprAST> parseStmtList() {
    std::vector<std::unique_ptr<StmtExprAST>> stmts;
    do {
        stmts.push_back(parseStmt());
    } while (CurTok.getType() == TOK_HAS || CurTok.getType() == TOK_IDENTIFIER);
    return std::make_unique<StmtLstExprAST>(stmts);
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

    if (isSingleStatement) {
        parseStmtList();
    } else {
        parseContent();
    }

    // Test lexer
    /*Token next;
    while ((next = getNextToken()).getType() != TOK_EOF) {
        std::cout << "Got token: " << std::to_string(next.getType()) << std::endl;
        std::cout << "Value: " << next.getValue() << std::endl;
        std::cout << "Loc: " << next.getCodePos() << std::endl;
        std::cout << std::endl;
    }*/
}