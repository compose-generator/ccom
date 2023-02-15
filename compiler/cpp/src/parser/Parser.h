/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <lexer/Lexer.h>
#include <memory>
#include <parser/ASTNodes.h>
#include <stdexcept>

class Parser {
public:
  // Constructors
  Parser() = default;
  Parser(bool, const std::string &, const std::string &, const std::string &, const std::string &);

  // Public methods
  ASTRootNode *parseAST();

private:
  // Private methods
  std::unique_ptr<ASTContentExprNode> parseContent();
  std::unique_ptr<ASTArbitraryExprNode> parseArbitrary();
  std::unique_ptr<ASTPayloadExprNode> parsePayload();
  std::unique_ptr<ASTSectionExprNode> parseSection();
  std::unique_ptr<ASTStmtListNode> parseStmtList();
  std::unique_ptr<ASTStmtNode> parseStmt();
  std::unique_ptr<ASTHasStmtNode> parseHasStmt();
  std::unique_ptr<ASTStmtNode> parseCompOrContainsStmt();
  std::unique_ptr<ASTCompStmtNode> parseCompStmt(std::unique_ptr<ASTKeyExprNode>);
  std::unique_ptr<ASTContainsStmtNode> parseContainsStmt(std::unique_ptr<ASTKeyExprNode>);
  std::unique_ptr<ASTIfBlockExprNode> parseIfBlockComLine();
  std::unique_ptr<ASTIfBlockExprNode> parseIfBlockComBlock();
  std::unique_ptr<ASTComLineBlockExprNode> parseComLineBlock();
  std::unique_ptr<ASTComBlockBlockExprNode> parseComBlockBlock();
  std::unique_ptr<ASTKeyExprNode> parseKey();
  std::unique_ptr<ASTIdentifierExprNode> parseIdentifier();
  std::unique_ptr<ASTValueExprNode> parseValue();
  std::unique_ptr<ASTStringExprNode> parseString();
  std::unique_ptr<ASTBooleanExprNode> parseBoolean();
  std::unique_ptr<ASTNumberExprNode> parseNumber();
  Operator parseOperator();

  // Members
  bool isSingleStatement;
  Lexer lexer;
};