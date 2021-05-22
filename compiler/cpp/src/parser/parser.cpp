// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include "parser.h"

// Token buffer
Token curTok;
Token getNextToken() { return curTok = getTok(); }

void expectToken(int tokenType) {
    if (curTok.getType() != tokenType) throw std::runtime_error("Syntax error at " + curTok.getCodePos());
    getNextToken();
}

std::unique_ptr<ArbitraryExprAST> parseArbitrary() {
    std::string value = curTok.getValue();
    getNextToken(); // Consume arbitrary string
    return std::make_unique<ArbitraryExprAST>(value);
}

std::unique_ptr<NumberExprAST> parseNumber() {
    int value = stoi(curTok.getValue());
    getNextToken(); // Consume number literal
    return std::make_unique<NumberExprAST>(value);
}

std::unique_ptr<BooleanExprAST> parseBoolean() {
    bool value = curTok.getValue() == "true";
    getNextToken(); // Consume boolean literal
    return std::make_unique<BooleanExprAST>(value);
}

std::unique_ptr<StringExprAST> parseString() {
    std::string value = curTok.getValue();
    getNextToken(); // Consume string literal
    return std::make_unique<StringExprAST>(value);
}

std::unique_ptr<ValueExprAST> parseValue() {
    switch (curTok.getType()) {
        case TOK_NUMBER:
            return parseNumber(); // Consume number
        case TOK_TRUE:
        case TOK_FALSE:
            return parseBoolean(); // Consume boolean
        case TOK_STRING:
            return parseString(); // Consume string
        default:
            // Should never happen
            throw std::runtime_error("Invalid token. Expected number, boolean or string at " + curTok.getCodePos() + " got" + std::to_string(curTok.getType()));
    }
}

std::unique_ptr<IdentifierExprAST> parseIdentifier() {
    std::string name = curTok.getValue();
    getNextToken(); // Consume identifier
    if (curTok.getType() != TOK_INDEX) return std::make_unique<IdentifierExprAST>(name);
    int idx = std::stoi(curTok.getValue());
    getNextToken(); // Consume index
    return std::make_unique<IdentifierExprAST>(name, idx);
}

std::unique_ptr<KeyExprAST> parseKey() {
    std::vector<std::unique_ptr<IdentifierExprAST>> identifiers;
    identifiers.push_back(parseIdentifier()); // consume first identifier
    while (curTok.getType() == TOK_DOT) {
        expectToken(TOK_DOT); // Consume '.'
        identifiers.push_back(parseIdentifier()); // consume identifier
    }
    return std::make_unique<KeyExprAST>(std::move(identifiers));
}

std::unique_ptr<CompStmtExprAST> parseCompStmt() {
    std::unique_ptr<KeyExprAST> key = parseKey(); // Consume key
    Operator op;
    switch (curTok.getType()) {
        case TOK_EQUALS:
            op = OP_EQUALS;
            break;
        case TOK_NOT_EQUALS:
            op = OP_NOT_EQUALS;
            break;
        default:
            throw std::runtime_error("Unknown comparison operator at " + curTok.getCodePos());
    }
    getNextToken(); // Consume operator
    std::unique_ptr<ValueExprAST> value = parseValue(); // Consume value
    return std::make_unique<CompStmtExprAST>(std::move(key), op, std::move(value));
}

std::unique_ptr<HasStmtExprAST> parseHasStmt() {
    bool inverted = false;
    if (curTok.getType() == TOK_NOT) {
        inverted = true;
        getNextToken(); // Consume 'not'
    }
    expectToken(TOK_HAS); // consume 'has'
    std::unique_ptr<KeyExprAST> key = parseKey(); // consume key
    return std::make_unique<HasStmtExprAST>(std::move(key), inverted);
}

std::unique_ptr<StmtExprAST> parseStmt() {
    if (curTok.getType() == TOK_HAS) return parseHasStmt(); // Consume HasStmt
    return parseCompStmt(); // Consume CompStmt
}

std::unique_ptr<StmtLstExprAST> parseStmtList() {
    std::vector<std::unique_ptr<StmtExprAST>> stmts;
    stmts.push_back(parseStmt()); // Consume first statement
    while (curTok.getType() == TOK_OR) {
        expectToken(TOK_OR); // Consume '|'
        stmts.push_back(parseStmt()); // Consume statement
    }
    return std::make_unique<StmtLstExprAST>(std::move(stmts));
}

std::unique_ptr<PayloadExprAST> parsePayload() {
    std::string value = curTok.getValue();
    expectToken(TOK_ARBITRARY); // Consume payload
    return std::make_unique<PayloadExprAST>(value);
}

std::unique_ptr<IfBlockExprAST> parseIfBlock() {
    expectToken(TOK_IF); // Consume 'if'
    std::unique_ptr<StmtLstExprAST> stmtList = parseStmtList(); // Consume stmtList
    expectToken(TOK_BRACE_OPEN); // Consume '{'
    std::unique_ptr<PayloadExprAST> payload = parsePayload(); // Consume payload
    expectToken(TOK_BRACE_CLOSE); // Consume '}'
    return std::make_unique<IfBlockExprAST>(std::move(stmtList), std::move(payload));
}

std::unique_ptr<ComBlockBlockExprAST> parseComBlockBlock() {
    expectToken(TOK_COM_BLOCK_IDEN_OPEN); // Consume ComBlockIdenOpen
    std::unique_ptr<IfBlockExprAST> ifBlock = parseIfBlock(); // Consume if block
    expectToken(TOK_COM_BLOCK_IDEN_CLOSE); // Consume ComBlockIdenClose
    return std::make_unique<ComBlockBlockExprAST>(std::move(ifBlock));
}

std::unique_ptr<ComLineBlockExprAST> parseComLineBlock() {
    expectToken(TOK_COM_LINE_IDEN); // Consume ComLineIden
    expectToken(TOK_IF); // Consume 'if'
    std::unique_ptr<StmtLstExprAST> stmtList = parseStmtList();  // Consume stmtList
    if (curTok.getType() == TOK_COM_LINE_IDEN) expectToken(TOK_COM_LINE_IDEN); // Consume ComLineIden optional
    expectToken(TOK_BRACE_OPEN); // Consume '{'
    std::unique_ptr<PayloadExprAST> payload = parsePayload(); // Consume payload
    expectToken(TOK_COM_LINE_IDEN); // Consume ComLineIden
    expectToken(TOK_BRACE_CLOSE); // Consume '}'
    return std::make_unique<ComLineBlockExprAST>(std::move(stmtList), std::move(payload));
}

std::unique_ptr<SectionExprAST> parseSection() {
    std::vector<std::unique_ptr<ComBlockExprAST>> comBlocks;
    bool isLineBlock;
    while ((isLineBlock = curTok.getType() == TOK_COM_LINE_IDEN) || curTok.getType() == TOK_COM_BLOCK_IDEN_OPEN) {
        if (isLineBlock) {
            comBlocks.push_back(parseComLineBlock()); // Consume ComLineBlock
            continue;
        }
        comBlocks.push_back(parseComBlockBlock()); // Consume ComBlockBlock
    }
    return std::make_unique<SectionExprAST>(std::move(comBlocks));
}

std::unique_ptr<ContentExprAST> parseContent() {
    std::vector<std::unique_ptr<ExprAST>> sections;
    while (curTok.getType() != TOK_EOF) {
        if (curTok.getType() == TOK_ARBITRARY) {
            sections.push_back(parseArbitrary()); // Consume arbitrary string
            continue;
        }
        sections.push_back(parseSection()); // Consume section
    }
    return std::make_unique<ContentExprAST>(std::move(sections));
}

TopLevelExprAST* executeSyntaxAnalysis(bool isSingleStatement, const std::string& fileInput, const std::string& lineCommentChars,
                               const std::string& blockCommentCharsOpen, const std::string& blockCommentCharsClose) {
    initLexer(isSingleStatement, fileInput, lineCommentChars, blockCommentCharsOpen, blockCommentCharsClose);

    // Fill buffer with first token
    getNextToken();

    // Build AST
    TopLevelExprAST* ast;
    if (isSingleStatement) {
        ast = parseStmtList().release();
    } else {
        ast = parseContent().release();
    }
    return ast;
}