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

std::unique_ptr<ValueExprAST> parseValue() {
    if (CurTok.getType() == TOK_NUMBER) return parseNumber();
    return parseString();
}

std::unique_ptr<IdentifierExprAST> parseIdentifier() {
    std::string name = CurTok.getValue();
    getNextToken(); // Consume identifier
    return std::make_unique<IdentifierExprAST>(name);
}

std::unique_ptr<KeyExprAST> parseKey() {
    std::vector<std::unique_ptr<IdentifierExprAST>> identifiers;
    identifiers.push_back(parseIdentifier()); // consume first identifier
    while (CurTok.getType() == TOK_DOT) {
        expectToken(TOK_DOT);
        identifiers.push_back(parseIdentifier()); // consume identifier
    }
    return std::make_unique<KeyExprAST>(identifiers);
}

std::unique_ptr<CompStmtExprAST> parseCompStmt() {
    std::unique_ptr<KeyExprAST> key = parseKey();
    Operator op;
    switch (CurTok.getType()) {
        case TOK_EQUALS:
            op = OP_EQUALS;
            break;
        case TOK_NOT_EQUALS:
            op = OP_NOT_EQUALS;
            break;
        default:
            throw std::runtime_error("Unknown comparison operator at " + CurTok.getCodePos());
    }
    getNextToken(); // Consume operator
    std::unique_ptr<ValueExprAST> value = parseValue();
    return std::make_unique<CompStmtExprAST>(std::move(key), op, std::move(value));
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
    if (CurTok.getType() == TOK_HAS) return parseHasStmt();
    return parseCompStmt();
}

std::unique_ptr<StmtLstExprAST> parseStmtList() {
    std::vector<std::unique_ptr<StmtExprAST>> stmts;
    stmts.push_back(parseStmt()); // Consume first statement
    while (CurTok.getType() == TOK_OR) {
        expectToken(TOK_OR);
        stmts.push_back(parseStmt()); // Consume statement
    }
    return std::make_unique<StmtLstExprAST>(stmts);
}

std::unique_ptr<PayloadExprAST> parsePayload() {
    std::string value = CurTok.getValue();
    expectToken(TOK_ARBITRARY); // Consume payload
    return std::make_unique<PayloadExprAST>(value);
}

std::unique_ptr<IfExprAST> parseIfBlock() {
    expectToken(TOK_IF); // Consume 'if'
    std::unique_ptr<StmtLstExprAST> stmtList = parseStmtList(); // Consume StmtList
    expectToken(TOK_BRACE_OPEN); // Consume '{'
    std::unique_ptr<PayloadExprAST> payload = parsePayload(); // Consume payload
    expectToken(TOK_BRACE_CLOSE); // Consume '}'
    return std::make_unique<IfExprAST>(std::move(stmtList), std::move(payload));
}

std::unique_ptr<ComBlockBlockExprAST> parseComBlockBlock() {
    expectToken(TOK_COM_BLOCK_IDEN_OPEN); // Consume ComBlockIdenOpen
    std::vector<std::unique_ptr<IfExprAST>> ifBlocks;
    while (CurTok.getType() == TOK_IF) {
        ifBlocks.push_back(parseIfBlock()); // Consume IfBlock
    }
    expectToken(TOK_COM_BLOCK_IDEN_CLOSE); // Consume ComBlockIdenClose
    return std::make_unique<ComBlockBlockExprAST>(std::move(ifBlocks));
}

std::unique_ptr<ComLineBlockExprAST> parseComLineBlock() {
    expectToken(TOK_COM_LINE_IDEN); // Consume ComLineIden
    std::unique_ptr<StmtLstExprAST> stmtList = parseStmtList();  // Consume StmtList
    if (CurTok.getType() == TOK_COM_LINE_IDEN) expectToken(TOK_COM_LINE_IDEN); // Consume ComLineIden optional
    expectToken(TOK_BRACE_OPEN); // Consume '{'
    std::unique_ptr<PayloadExprAST> payload = parsePayload(); // Consume payload
    expectToken(TOK_COM_LINE_IDEN); // Consume ComLineIden
    expectToken(TOK_BRACE_CLOSE); // Consume '}'
    return std::make_unique<ComLineBlockExprAST>(std::move(stmtList), std::move(payload));
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