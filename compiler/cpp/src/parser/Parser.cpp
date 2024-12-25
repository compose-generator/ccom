/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#include "Parser.h"

Parser::Parser(bool isSingleStatement, const std::string &fileInput, const std::string &inputCommentLineIdentifiers,
               const std::string &inputCommentBlockOpenIdentifiers, const std::string &inputCommentBlockCloseIdentifiers) {
  this->isSingleStatement = isSingleStatement;

  lexer = Lexer(isSingleStatement, fileInput, inputCommentLineIdentifiers, inputCommentBlockOpenIdentifiers,
                inputCommentBlockCloseIdentifiers);
}

ASTRootNode *Parser::parseAST() {
  if (isSingleStatement)
    return parseStmtList().release();
  return parseContent().release();
}

std::unique_ptr<ASTContentExprNode> Parser::parseContent() {
  std::vector<std::unique_ptr<ASTContentBlockExprNode>> contentBlocks;
  while (lexer.getLookahead().getType() != TOK_EOF) {
    if (lexer.getLookahead().getType() == TOK_ARBITRARY) {
      contentBlocks.push_back(parseArbitrary()); // Consume arbitrary string
      continue;
    }
    contentBlocks.push_back(parseSection()); // Consume section
  }
  return std::make_unique<ASTContentExprNode>(std::move(contentBlocks));
}

std::unique_ptr<ASTArbitraryExprNode> Parser::parseArbitrary() {
  std::string value = lexer.getLookahead().getValue();
  lexer.advance(); // Consume arbitrary string
  return std::make_unique<ASTArbitraryExprNode>(value);
}

std::unique_ptr<ASTPayloadExprNode> Parser::parsePayload() {
  std::string value = lexer.getLookahead().getValue();
  lexer.expect(TOK_ARBITRARY); // Consume payload
  return std::make_unique<ASTPayloadExprNode>(value);
}

std::unique_ptr<ASTSectionExprNode> Parser::parseSection() {
  std::vector<std::unique_ptr<ASTComBlockExprNode>> comBlocks;
  bool isLineBlock;
  while ((isLineBlock = lexer.getLookahead().getType() == TOK_COM_LINE_IDEN) ||
         lexer.getLookahead().getType() == TOK_COM_BLOCK_IDEN_OPEN) {
    if (isLineBlock) {
      comBlocks.push_back(parseComLineBlock()); // Consume ComLineBlock
      continue;
    }
    comBlocks.push_back(parseComBlockBlock()); // Consume ComBlockBlock
  }
  return std::make_unique<ASTSectionExprNode>(std::move(comBlocks));
}

std::unique_ptr<ASTStmtListNode> Parser::parseStmtList() {
  std::vector<std::unique_ptr<ASTStmtNode>> stmts;
  stmts.push_back(parseStmt()); // Consume first statement
  while (lexer.getLookahead().getType() == TOK_OR) {
    lexer.expect(TOK_OR);         // Consume '|'
    stmts.push_back(parseStmt()); // Consume statement
  }
  return std::make_unique<ASTStmtListNode>(std::move(stmts));
}

std::unique_ptr<ASTStmtNode> Parser::parseStmt() {
  if (lexer.getLookahead().getType() == TOK_HAS || lexer.getLookahead().getType() == TOK_NOT)
    return parseHasStmt();          // Consume HasStmt
  return parseCompOrContainsStmt(); // Consume CompStmt or ContainsStmt
}

std::unique_ptr<ASTHasStmtNode> Parser::parseHasStmt() {
  bool inverted = false;
  if (lexer.getLookahead().getType() == TOK_NOT) {
    inverted = true;
    lexer.advance(); // Consume 'not'
  }
  lexer.expect(TOK_HAS);                            // consume 'has'
  std::unique_ptr<ASTKeyExprNode> key = parseKey(); // consume key
  return std::make_unique<ASTHasStmtNode>(std::move(key), inverted);
}

std::unique_ptr<ASTStmtNode> Parser::parseCompOrContainsStmt() {
  std::unique_ptr<ASTKeyExprNode> key = parseKey(); // Consume key
  // Decide how to proceed depending on the next token
  if (lexer.getLookahead().getType() == TOK_CONTAINS || lexer.getLookahead().getType() == TOK_NOT)
    return parseContainsStmt(std::move(key)); // Consume ContainsStmt
  return parseCompStmt(std::move(key));
}

std::unique_ptr<ASTCompStmtNode> Parser::parseCompStmt(std::unique_ptr<ASTKeyExprNode> key) {
  Operator op = parseOperator();                          // Consume operator
  std::unique_ptr<ASTValueExprNode> value = parseValue(); // Consume value
  return std::make_unique<ASTCompStmtNode>(std::move(key), op, std::move(value));
}

std::unique_ptr<ASTContainsStmtNode> Parser::parseContainsStmt(std::unique_ptr<ASTKeyExprNode> listKey) {
  bool inverted = false;
  if (lexer.getLookahead().getType() == TOK_NOT) {
    inverted = true;
    lexer.advance(); // Consume 'not'
  }
  lexer.expect(TOK_CONTAINS);                             // consume 'contains'
  std::unique_ptr<ASTKeyExprNode> valueKey = parseKey();  // consume value key
  Operator op = parseOperator();                          // Consume operator
  std::unique_ptr<ASTValueExprNode> value = parseValue(); // consume value
  return std::make_unique<ASTContainsStmtNode>(std::move(listKey), std::move(valueKey), std::move(value), op, inverted);
}

std::unique_ptr<ASTIfBlockExprNode> Parser::parseIfBlockComLine() {
  lexer.expect(TOK_IF);                                        // Consume 'if'
  std::unique_ptr<ASTStmtListNode> stmtList = parseStmtList(); // Consume stmtList
  if (lexer.getLookahead().getType() == TOK_COM_LINE_IDEN)
    lexer.expect(TOK_COM_LINE_IDEN);                            // Consume ComLineIden optional
  lexer.expect(TOK_BRACE_OPEN);                                 // Consume '{'
  std::unique_ptr<ASTPayloadExprNode> payload = parsePayload(); // Consume payload
  lexer.expect(TOK_COM_LINE_IDEN);                              // Consume ComLineIden
  lexer.expect(TOK_BRACE_CLOSE);                                // Consume '}'
  return std::make_unique<ASTIfBlockExprNode>(std::move(stmtList), std::move(payload));
}

std::unique_ptr<ASTIfBlockExprNode> Parser::parseIfBlockComBlock() {
  lexer.expect(TOK_IF);                                         // Consume 'if'
  std::unique_ptr<ASTStmtListNode> stmtList = parseStmtList();  // Consume stmtList
  lexer.expect(TOK_BRACE_OPEN);                                 // Consume '{'
  std::unique_ptr<ASTPayloadExprNode> payload = parsePayload(); // Consume payload
  lexer.expect(TOK_BRACE_CLOSE);                                // Consume '}'
  return std::make_unique<ASTIfBlockExprNode>(std::move(stmtList), std::move(payload));
}

std::unique_ptr<ASTComLineBlockExprNode> Parser::parseComLineBlock() {
  lexer.expect(TOK_COM_LINE_IDEN);                                     // Consume ComLineIden
  std::unique_ptr<ASTIfBlockExprNode> ifBlock = parseIfBlockComLine(); // Consume if block
  return std::make_unique<ASTComLineBlockExprNode>(std::move(ifBlock));
}

std::unique_ptr<ASTComBlockBlockExprNode> Parser::parseComBlockBlock() {
  lexer.expect(TOK_COM_BLOCK_IDEN_OPEN);                                // Consume ComBlockIdenOpen
  std::unique_ptr<ASTIfBlockExprNode> ifBlock = parseIfBlockComBlock(); // Consume if block
  lexer.expect(TOK_COM_BLOCK_IDEN_CLOSE);                               // Consume ComBlockIdenClose
  return std::make_unique<ASTComBlockBlockExprNode>(std::move(ifBlock));
}

std::unique_ptr<ASTKeyExprNode> Parser::parseKey() {
  std::vector<std::unique_ptr<ASTIdentifierExprNode>> identifiers;
  identifiers.push_back(parseIdentifier()); // consume first identifier
  while (lexer.getLookahead().getType() == TOK_DOT) {
    lexer.expect(TOK_DOT);                    // Consume '.'
    identifiers.push_back(parseIdentifier()); // consume identifier
  }
  return std::make_unique<ASTKeyExprNode>(std::move(identifiers));
}

std::unique_ptr<ASTIdentifierExprNode> Parser::parseIdentifier() {
  std::string name = lexer.getLookahead().getValue();
  lexer.advance(); // Consume identifier
  if (lexer.getLookahead().getType() != TOK_INDEX)
    return std::make_unique<ASTIdentifierExprNode>(name);
  int idx = std::stoi(lexer.getLookahead().getValue());
  lexer.advance(); // Consume index
  return std::make_unique<ASTIdentifierExprNode>(name, idx);
}

std::unique_ptr<ASTValueExprNode> Parser::parseValue() {
  switch (lexer.getLookahead().getType()) {
  case TOK_NUMBER:
    return parseNumber(); // Consume number
  case TOK_TRUE:          // fall-through
  case TOK_FALSE:
    return parseBoolean(); // Consume boolean
  case TOK_STRING:
    return parseString(); // Consume string
  default:
    // Should never happen
    throw std::runtime_error("Invalid token. Expected number, boolean or string at " + lexer.getLookahead().getCodePos() +
                             " got" + std::to_string(lexer.getLookahead().getType()));
  }
}

std::unique_ptr<ASTStringExprNode> Parser::parseString() {
  std::string value = lexer.getLookahead().getValue();
  lexer.advance(); // Consume string literal
  return std::make_unique<ASTStringExprNode>(value);
}

std::unique_ptr<ASTBooleanExprNode> Parser::parseBoolean() {
  bool value = lexer.getLookahead().getType() == TOK_TRUE;
  lexer.advance(); // Consume boolean literal
  return std::make_unique<ASTBooleanExprNode>(value);
}

std::unique_ptr<ASTNumberExprNode> Parser::parseNumber() {
  int value = stoi(lexer.getLookahead().getValue());
  lexer.advance(); // Consume number literal
  return std::make_unique<ASTNumberExprNode>(value);
}

Operator Parser::parseOperator() {
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
  return op;
}