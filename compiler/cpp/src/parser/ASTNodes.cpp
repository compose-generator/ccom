/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#include "ASTNodes.h"

std::string ASTRootNode::serialize() {
  switch (type) {
  case CONTENT_EXPR: {
    auto *contentExpr = static_cast<ASTContentExprNode *>(this);
    return contentExpr->serialize();
  }
  case STMT_LST_EXPR: {
    auto *stmtLstExpr = static_cast<ASTContentExprNode *>(this);
    return stmtLstExpr->serialize();
  }
  default:
    throw std::runtime_error("Unknown top level expression type");
  }
}

std::string ASTIdentifierExprNode::serialize() const {
  return "identifier(name: '" + name + "', index: '" + std::to_string(index) + "')";
}

std::string ASTValueExprNode::serialize() {
  switch (type) {
  case NUMBER_EXPR: {
    auto *numberExpr = static_cast<ASTNumberExprNode *>(this);
    return numberExpr->serialize();
  }
  case BOOLEAN_EXPR: {
    auto *booleanExpr = static_cast<ASTBooleanExprNode *>(this);
    return booleanExpr->serialize();
  }
  case STRING_EXPR: {
    auto *stringExprAST = static_cast<ASTStringExprNode *>(this);
    return stringExprAST->serialize();
  }
  default:
    throw std::runtime_error("Unknown value type");
  }
}

std::string ASTKeyExprNode::serialize() const {
  std::string result = "key(";
  for (auto &identifier : identifiers)
    result += identifier->serialize();
  return result + ")";
}

std::string ASTStmtNode::serialize() {
  switch (type) {
  case HAS_STMT_EXPR: {
    auto *hasStmt = static_cast<ASTHasStmtNode *>(this);
    return hasStmt->serialize();
  }
  case COMP_STMT_EXPR: {
    auto *compStmt = static_cast<ASTCompStmtNode *>(this);
    return compStmt->serialize();
  }
  case CONTAINS_STMT_EXPR: {
    auto *containsStmt = static_cast<ASTContainsStmtNode *>(this);
    return containsStmt->serialize();
  }
  default:
    throw std::runtime_error("Unknown value type");
  }
}

std::string ASTStmtListNode::serialize() const {
  std::string serializedStatements;
  for (auto &stmt : stmts)
    serializedStatements += stmt->serialize();
  return "stmt-lst(" + serializedStatements + ")";
}

std::string ASTContentBlockExprNode::serialize() {
  switch (type) {
  case SECTION_EXPR: {
    auto *sectionExpr = static_cast<ASTSectionExprNode *>(this);
    return sectionExpr->serialize();
  }
  case ARBITRARY_EXPR: {
    auto *arbitraryExpr = static_cast<ASTArbitraryExprNode *>(this);
    return arbitraryExpr->serialize();
  }
  default:
    throw std::runtime_error("Unknown content block type");
  }
}

std::string ASTContentExprNode::serialize() const {
  std::string serializedContentBlocks;
  for (auto &contentBlock : contentBlocks)
    serializedContentBlocks += contentBlock->serialize();
  return "content(" + serializedContentBlocks + ")";
}

std::string ASTArbitraryExprNode::serialize() const { return "arbitrary(value: '" + value + "')"; }

std::string ASTPayloadExprNode::serialize() const { return "payload(value: '" + value + "')"; }

std::string ASTIfBlockExprNode::serialize() const {
  return "if(stmt-lst: " + stmtList->serialize() + ", payload: " + payload->serialize() + ")";
}

std::string ASTComBlockExprNode::serialize() {
  switch (type) {
  case COM_LINE_BLOCK_EXPR: {
    auto *comLineBlock = static_cast<ASTComLineBlockExprNode *>(this);
    return comLineBlock->serialize();
  }
  case COM_BLOCK_BLOCK_EXPR: {
    auto *comBlockBlock = static_cast<ASTComBlockBlockExprNode *>(this);
    return comBlockBlock->serialize();
  }
  default:
    throw std::runtime_error("Unknown com block type");
  }
}

std::string ASTComLineBlockExprNode::serialize() const { return "com-line-block(" + ifBlock->serialize() + ")"; }

std::string ASTComBlockBlockExprNode::serialize() const { return "com-block-block(" + ifBlock->serialize() + ")"; }

std::string ASTCompStmtNode::serialize() const {
  std::string serializedValue;
  switch (value->type) {
  case ASTBooleanExprNode::NUMBER_EXPR: {
    auto *numberExpr = static_cast<ASTNumberExprNode *>(value.get());
    serializedValue = numberExpr->serialize();
    break;
  }
  case ASTBooleanExprNode::BOOLEAN_EXPR: {
    auto *booleanExpr = static_cast<ASTBooleanExprNode *>(value.get());
    serializedValue = booleanExpr->serialize();
    break;
  }
  case ASTBooleanExprNode::STRING_EXPR: {
    auto *stringExprAST = static_cast<ASTNumberExprNode *>(value.get());
    serializedValue = stringExprAST->serialize();
    break;
  }
  default:
    throw std::runtime_error("Unknown value type");
  }
  return "comp-stmt(key: " + key->serialize() + ", operator: " + std::to_string(op) + ", value: " + serializedValue + ")";
}

std::string ASTContainsStmtNode::serialize() const {
  return "contains-stmt(list-key: " + listKey->serialize() + ", value-key: " + valueKey->serialize() +
         ", operator: " + std::to_string(op) + ", value: " + value->serialize() + ")";
}

std::string ASTHasStmtNode::serialize() const {
  return "has-stmt(key: " + key->serialize() + ", isInverted: " + std::to_string(isInverted) + ")";
}

std::string ASTNumberExprNode::serialize() const { return "number(value: '" + std::to_string(value) + "')"; }

std::string ASTSectionExprNode::serialize() const {
  std::string serializedComBlocks;
  for (auto &comBlock : comBlocks)
    serializedComBlocks += comBlock->serialize();
  return "section(" + serializedComBlocks + ")";
}

std::string ASTStringExprNode::serialize() const { return "string(value: '" + value + "')"; }

std::string ASTBooleanExprNode::serialize() const { return "boolean(value: '" + std::to_string(value) + "')"; }