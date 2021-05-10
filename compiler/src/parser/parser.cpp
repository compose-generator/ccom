// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#include "parser.h"

// Token buffer
Token CurTok;
Token getNextToken() { return CurTok = getTok(); }

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

void expectToken(int tokenType) {
    if (CurTok.getType() != tokenType) throw std::runtime_error("Syntax error at " + CurTok.getCodePos());
    getNextToken();
}

std::unique_ptr<ArbitraryExprAST> parseArbitrary() {
    std::string value = CurTok.getValue();
    getNextToken(); // Consume arbitrary string
    return std::make_unique<ArbitraryExprAST>(value);
}

std::unique_ptr<NumberExprAST> parseNumber() {
    int value = stoi(CurTok.getValue());
    getNextToken(); // Consume number literal
    return std::make_unique<NumberExprAST>(value);
}

std::unique_ptr<StringExprAST> parseString() {
    std::string value = CurTok.getValue();
    getNextToken(); // Consume string literal
    return std::make_unique<StringExprAST>(value);
}

std::unique_ptr<ValueExprAST> parseValue() {
    if (CurTok.getType() == TOK_NUMBER) return parseNumber(); // Consume number
    return parseString(); // Consume string
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
        expectToken(TOK_DOT); // Consume '.'
        identifiers.push_back(parseIdentifier()); // consume identifier
    }
    return std::make_unique<KeyExprAST>(std::move(identifiers));
}

std::unique_ptr<CompStmtExprAST> parseCompStmt() {
    std::unique_ptr<KeyExprAST> key = parseKey(); // Consume key
    Operator op;
    switch (CurTok.getType()) {
        case TOK_EQUALS:
            op = OP_EQUALS;
            break;
        case TOK_NOT_EQUALS:
            op = OP_NOT_EQUALS;
            break;
        default:
            std::cout << "Operator: " << CurTok.getType() << std::endl;
            throw std::runtime_error("Unknown comparison operator at " + CurTok.getCodePos());
    }
    getNextToken(); // Consume operator
    std::unique_ptr<ValueExprAST> value = parseValue(); // Consume value
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
    if (CurTok.getType() == TOK_HAS) return parseHasStmt(); // Consume HasStmt
    return parseCompStmt(); // Consume CompStmt
}

std::unique_ptr<StmtLstExprAST> parseStmtList() {
    std::vector<std::unique_ptr<StmtExprAST>> stmts;
    stmts.push_back(parseStmt()); // Consume first statement
    while (CurTok.getType() == TOK_OR) {
        expectToken(TOK_OR); // Consume '|'
        stmts.push_back(parseStmt()); // Consume statement
    }
    return std::make_unique<StmtLstExprAST>(std::move(stmts));
}

std::unique_ptr<PayloadExprAST> parsePayload() {
    std::string value = CurTok.getValue();
    expectToken(TOK_ARBITRARY); // Consume payload
    return std::make_unique<PayloadExprAST>(value);
}

std::unique_ptr<IfBlockExprAST> parseIfBlock() {
    expectToken(TOK_IF); // Consume 'if'
    std::unique_ptr<StmtLstExprAST> stmtList = parseStmtList(); // Consume StmtList
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
    std::unique_ptr<StmtLstExprAST> stmtList = parseStmtList();  // Consume StmtList
    if (CurTok.getType() == TOK_COM_LINE_IDEN) expectToken(TOK_COM_LINE_IDEN); // Consume ComLineIden optional
    expectToken(TOK_BRACE_OPEN); // Consume '{'
    std::unique_ptr<PayloadExprAST> payload = parsePayload(); // Consume payload
    expectToken(TOK_COM_LINE_IDEN); // Consume ComLineIden
    expectToken(TOK_BRACE_CLOSE); // Consume '}'
    return std::make_unique<ComLineBlockExprAST>(std::move(stmtList), std::move(payload));
}

std::unique_ptr<SectionExprAST> parseSection() {
    std::vector<std::unique_ptr<ComBlockExprAST>> comBlocks;
    bool isLineBlock;
    while ((isLineBlock = CurTok.getType() == TOK_COM_LINE_IDEN) || CurTok.getType() == TOK_COM_BLOCK_IDEN_OPEN) {
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
    while (CurTok.getType() != TOK_EOF) {
        if (CurTok.getType() == TOK_ARBITRARY) {
            sections.push_back(parseArbitrary()); // Consume arbitrary string
            continue;
        }
        sections.push_back(parseSection()); // Consume section
    }
    return std::make_unique<ContentExprAST>(std::move(sections));
}

void initParser(const bool isSingleStatement, const std::string& fileInput, const std::string& dataInput,
                const std::string& lineCommentChars, const std::string& blockCommentCharsOpen,
                const std::string& blockCommentCharsClose) {
    initLexer(fileInput, lineCommentChars, blockCommentCharsOpen, blockCommentCharsClose);

    // Fill buffer with first token
    getNextToken();

    // Build AST
    if (isSingleStatement) {
        StmtLstExprAST* stmtList = parseStmtList().release();
    } else {
        ContentExprAST* content = parseContent().release();
        std::cout << "Number of sections: " << content->GetSections().size() << std::endl;
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