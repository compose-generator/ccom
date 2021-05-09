// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include "parser.h"

// Token buffer
Token CurTok;
Token getNextToken() { return CurTok = getTok(); }

void expectToken(int tokenType) {
    if (CurTok.getType() != tokenType) throw std::runtime_error("Syntax error  at " + CurTok.getCodePos());
    getNextToken();
}

std::unique_ptr<NumberExprAST> parseNumber() {
    int value = stoi(CurTok.getValue());
    getNextToken(); // Consume number literal
    return std::make_unique<NumberExprAST>(value);
}

std::unique_ptr<StringExprAST> parseString() {
    std::string value = CurTok.getValue();
    getNextToken();
    return std::make_unique<StringExprAST>(value);
}

std::unique_ptr<ExprAST> parseValue() {
    if (CurTok.getType() == TOK_NUMBER) {

    } else {

    }
}

std::unique_ptr<IdentifierExprAST> parseIdentifier() {
    std::string name = CurTok.getValue();
    getNextToken(); // Consume identifier
    return std::make_unique<IdentifierExprAST>(name);
}

std::unique_ptr<KeyExprAST> parseKey() {
    std::vector<std::unique_ptr<IdentifierExprAST>> identifiers;
    do {

    } while (CurTok.getType() == TOK_DOT);
    return std::make_unique<KeyExprAST>(identifiers);
}

std::unique_ptr<ExprAST> parseCompStmt() {

}

std::unique_ptr<HasStmtExprAST> parseHasStmt() {
    bool inverted = false;
    if (CurTok.getType() == TOK_NOT) {
        inverted = true;
        getNextToken(); // Consume 'not'
    }
    expectToken(TOK_HAS); // consume 'has'
    std::unique_ptr<KeyExprAST> key = parseKey(); // consume key
    return std::make_unique<HasStmtExprAST>(std::move(key), inverted);
}

std::unique_ptr<StmtExprAST> parseStmt() {
    if (CurTok.getType() == TOK_HAS) {

    } else {

    }
}

std::unique_ptr<StmtLstExprAST> parseStmtList() {
    std::vector<std::unique_ptr<StmtExprAST>> stmts;
    do {
        stmts.push_back(parseStmt());
    } while (CurTok.getType() == TOK_HAS || CurTok.getType() == TOK_IDENTIFIER);
    return std::make_unique<StmtLstExprAST>(stmts);
}

std::unique_ptr<PayloadExprAST> parsePayload() {
    std::string value = CurTok.getValue();
    getNextToken(); // Consume payload
    return std::make_unique<PayloadExprAST>(value);
}

std::unique_ptr<ExprAST> parseComBlockIdenClose() {
    expectToken(TOK_COM_BLOCK_IDEN_CLOSE);
}

std::unique_ptr<ExprAST> parseComBlockIdenOpen() {
    expectToken(TOK_COM_BLOCK_IDEN_OPEN);
}

std::unique_ptr<ExprAST> parseComIdenPayload() {
    expectToken(TOK_COM_IDEN_PAYLOAD);
}

std::unique_ptr<ExprAST> parseComLineIden() {
    expectToken(TOK_COM_LINE_IDEN);
}

std::unique_ptr<IfExprAST> parseIfBlock() {
    expectToken(TOK_IF);
    std::unique_ptr<StmtLstExprAST> stmtList = parseStmtList();
    expectToken(TOK_BRACE_OPEN);
    std::unique_ptr<PayloadExprAST> payload = parsePayload();
    expectToken(TOK_BRACE_CLOSE);
    return std::make_unique<IfExprAST>(std::move(stmtList), std::move(payload));
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