//
// Created by Marc on 24.05.2021.
//

#include "Parser.h"

Parser::Parser(bool isSingleStatement, const std::string &fileInput, const std::string &inputCommentLineIdentifiers,
               const std::string &inputCommentBlockOpenIdentifiers, const std::string &inputCommentBlockCloseIdentifiers) {
    this->isSingleStatement = isSingleStatement;

    lexer = Lexer(isSingleStatement, fileInput, inputCommentLineIdentifiers,
                  inputCommentBlockOpenIdentifiers, inputCommentBlockCloseIdentifiers);
}

TopLevelExprAST *Parser::parseAST() {
    if (isSingleStatement) return parseStmtList().release();
    return parseContent().release();
}


std::unique_ptr<ContentExprAST> Parser::parseContent() {
    std::vector<std::unique_ptr<ContentBlockExprAST>> contentBlocks;
    while (lexer.getLookahead().getType() != TOK_EOF) {
        if (lexer.getLookahead().getType() == TOK_ARBITRARY) {
            contentBlocks.push_back(parseArbitrary()); // Consume arbitrary string
            continue;
        }
        contentBlocks.push_back(parseSection()); // Consume section
    }
    return std::make_unique<ContentExprAST>(std::move(contentBlocks));
}

std::unique_ptr<ArbitraryExprAST> Parser::parseArbitrary() {
    std::string value = lexer.getLookahead().getValue();
    lexer.advance(); // Consume arbitrary string
    return std::make_unique<ArbitraryExprAST>(value);
}

std::unique_ptr<PayloadExprAST> Parser::parsePayload() {
    std::string value = lexer.getLookahead().getValue();
    lexer.expect(TOK_ARBITRARY); // Consume payload
    return std::make_unique<PayloadExprAST>(value);
}

std::unique_ptr<SectionExprAST> Parser::parseSection() {
    std::vector<std::unique_ptr<ComBlockExprAST>> comBlocks;
    bool isLineBlock;
    while ((isLineBlock = lexer.getLookahead().getType() == TOK_COM_LINE_IDEN) ||
        lexer.getLookahead().getType() == TOK_COM_BLOCK_IDEN_OPEN) {
        if (isLineBlock) {
            comBlocks.push_back(parseComLineBlock()); // Consume ComLineBlock
            continue;
        }
        comBlocks.push_back(parseComBlockBlock()); // Consume ComBlockBlock
    }
    return std::make_unique<SectionExprAST>(std::move(comBlocks));
}

std::unique_ptr<StmtLstExprAST> Parser::parseStmtList() {
    std::vector<std::unique_ptr<StmtExprAST>> stmts;
    stmts.push_back(parseStmt()); // Consume first statement
    while (lexer.getLookahead().getType() == TOK_OR) {
        lexer.expect(TOK_OR); // Consume '|'
        stmts.push_back(parseStmt()); // Consume statement
    }
    return std::make_unique<StmtLstExprAST>(std::move(stmts));
}

std::unique_ptr<StmtExprAST> Parser::parseStmt() {
    if (lexer.getLookahead().getType() == TOK_HAS || lexer.getLookahead().getType() == TOK_NOT)
        return parseHasStmt(); // Consume HasStmt
    return parseCompStmt(); // Consume CompStmt
}

std::unique_ptr<HasStmtExprAST> Parser::parseHasStmt() {
    bool inverted = false;
    if (lexer.getLookahead().getType() == TOK_NOT) {
        inverted = true;
        lexer.advance(); // Consume 'not'
    }
    lexer.expect(TOK_HAS); // consume 'has'
    std::unique_ptr<KeyExprAST> key = parseKey(); // consume key
    return std::make_unique<HasStmtExprAST>(std::move(key), inverted);
}

std::unique_ptr<CompStmtExprAST> Parser::parseCompStmt() {
    std::unique_ptr<KeyExprAST> key = parseKey(); // Consume key
    Operator op;
    switch (lexer.getLookahead().getType()) {
        case TOK_EQUALS:
            op = OP_EQUALS;
            break;
        case TOK_NOT_EQUALS:
            op = OP_NOT_EQUALS;
            break;
        case TOK_GREATER:
            op = OP_GREATER;
            break;
        case TOK_LESS:
            op = OP_LESS;
            break;
        case TOK_GREATER_EQUAL:
            op = OP_GREATER_EQUAL;
            break;
        case TOK_LESS_EQUAL:
            op = OP_LESS_EQUAL;
            break;
        default:
            throw std::runtime_error("Unknown comparison operator at " + lexer.getLookahead().getCodePos());
    }
    lexer.advance(); // Consume operator
    std::unique_ptr<ValueExprAST> value = parseValue(); // Consume value
    return std::make_unique<CompStmtExprAST>(std::move(key), op, std::move(value));
}

std::unique_ptr<IfBlockExprAST> Parser::parseIfBlock() {
    lexer.expect(TOK_IF); // Consume 'if'
    std::unique_ptr<StmtLstExprAST> stmtList = parseStmtList(); // Consume stmtList
    lexer.expect(TOK_BRACE_OPEN); // Consume '{'
    std::unique_ptr<PayloadExprAST> payload = parsePayload(); // Consume payload
    lexer.expect(TOK_BRACE_CLOSE); // Consume '}'
    return std::make_unique<IfBlockExprAST>(std::move(stmtList), std::move(payload));
}

std::unique_ptr<ComLineBlockExprAST> Parser::parseComLineBlock() {
    lexer.expect(TOK_COM_LINE_IDEN); // Consume ComLineIden
    lexer.expect(TOK_IF); // Consume 'if'
    std::unique_ptr<StmtLstExprAST> stmtList = parseStmtList();  // Consume stmtList
    if (lexer.getLookahead().getType() == TOK_COM_LINE_IDEN)
        lexer.expect(TOK_COM_LINE_IDEN); // Consume ComLineIden optional
    lexer.expect(TOK_BRACE_OPEN); // Consume '{'
    std::unique_ptr<PayloadExprAST> payload = parsePayload(); // Consume payload
    lexer.expect(TOK_COM_LINE_IDEN); // Consume ComLineIden
    lexer.expect(TOK_BRACE_CLOSE); // Consume '}'
    return std::make_unique<ComLineBlockExprAST>(std::move(stmtList), std::move(payload));
}

std::unique_ptr<ComBlockBlockExprAST> Parser::parseComBlockBlock() {
    lexer.expect(TOK_COM_BLOCK_IDEN_OPEN); // Consume ComBlockIdenOpen
    std::unique_ptr<IfBlockExprAST> ifBlock = parseIfBlock(); // Consume if block
    lexer.expect(TOK_COM_BLOCK_IDEN_CLOSE); // Consume ComBlockIdenClose
    return std::make_unique<ComBlockBlockExprAST>(std::move(ifBlock));
}

std::unique_ptr<KeyExprAST> Parser::parseKey() {
    std::vector<std::unique_ptr<IdentifierExprAST>> identifiers;
    identifiers.push_back(parseIdentifier()); // consume first identifier
    while (lexer.getLookahead().getType() == TOK_DOT) {
        lexer.expect(TOK_DOT); // Consume '.'
        identifiers.push_back(parseIdentifier()); // consume identifier
    }
    return std::make_unique<KeyExprAST>(std::move(identifiers));
}

std::unique_ptr<IdentifierExprAST> Parser::parseIdentifier() {
    std::string name = lexer.getLookahead().getValue();
    lexer.advance(); // Consume identifier
    if (lexer.getLookahead().getType() != TOK_INDEX) return std::make_unique<IdentifierExprAST>(name);
    int idx = std::stoi(lexer.getLookahead().getValue());
    lexer.advance(); // Consume index
    return std::make_unique<IdentifierExprAST>(name, idx);
}

std::unique_ptr<ValueExprAST> Parser::parseValue() {
    switch (lexer.getLookahead().getType()) {
        case TOK_NUMBER:
            return parseNumber(); // Consume number
        case TOK_TRUE:
        case TOK_FALSE:
            return parseBoolean(); // Consume boolean
        case TOK_STRING:
            return parseString(); // Consume string
        default:
            // Should never happen
            throw std::runtime_error("Invalid token. Expected number, boolean or string at " +
                lexer.getLookahead().getCodePos() + " got" + std::to_string(lexer.getLookahead().getType()));
    }
}

std::unique_ptr<StringExprAST> Parser::parseString() {
    std::string value = lexer.getLookahead().getValue();
    lexer.advance(); // Consume string literal
    return std::make_unique<StringExprAST>(value);
}

std::unique_ptr<BooleanExprAST> Parser::parseBoolean() {
    bool value = lexer.getLookahead().getValue() == "true";
    lexer.advance(); // Consume boolean literal
    return std::make_unique<BooleanExprAST>(value);
}

std::unique_ptr<NumberExprAST> Parser::parseNumber() {
    int value = stoi(lexer.getLookahead().getValue());
    lexer.advance(); // Consume number literal
    return std::make_unique<NumberExprAST>(value);
}
