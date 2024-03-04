/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#include "Analyzer.h"

// -------------------------------------------------- Public functions -------------------------------------------------

Analyzer::Analyzer(bool isSingleStatement, const std::string &fileInput, JSONParser jsonParser,
                   const std::string &inputCommentLineIdentifiers, const std::string &inputCommentBlockOpenIdentifiers,
                   const std::string &inputCommentBlockCloseIdentifiers)
    : jsonParser(std::move(jsonParser)) {
  this->isSingleStatement = isSingleStatement;

  // Initialize parser
  parser = Parser(isSingleStatement, fileInput, inputCommentLineIdentifiers, inputCommentBlockOpenIdentifiers,
                  inputCommentBlockCloseIdentifiers);

  // Parse abstract syntax tree
  ast = parser.parseAST();
}

void Analyzer::executeAnalysis() {
  // Execute checks
  checkDataTypeCompatibility();
  // ToDo: Add more checks here
}

// ------------------------------------------------- Private functions -------------------------------------------------

void Analyzer::checkDataTypeCompatibility() {
  if (isSingleStatement) {
    if (ast->type != ASTRootNode::STMT_LST_EXPR)
      throw std::runtime_error("Input was no single statement list");
    auto *stmtLst = static_cast<ASTStmtListNode *>(ast);
    checkDataTypeCompatibilityStmtList(stmtLst);
  } else {
    auto *content = static_cast<ASTContentExprNode *>(ast);
    checkDataTypeCompatibilityContent(content);
  }
}

void Analyzer::checkDataTypeCompatibilityContent(ASTContentExprNode *content) {
  // Loop through sections
  for (const std::unique_ptr<ASTContentBlockExprNode> &contentBlock : content->contentBlocks) {
    if (contentBlock->type == ASTArbitraryExprNode::SECTION_EXPR) {
      auto *relevantSection = static_cast<ASTSectionExprNode *>(contentBlock.get());
      // Loop through comBlocks
      for (const std::unique_ptr<ASTComBlockExprNode> &comBlock : relevantSection->comBlocks) {
        switch (comBlock->type) {
        case ASTComBlockExprNode::COM_LINE_BLOCK_EXPR: {
          auto *lineBlockExpr = static_cast<ASTComLineBlockExprNode *>(comBlock.get());
          checkDataTypeCompatibilityStmtList(lineBlockExpr->ifBlock->stmtList.get());
          continue;
        }
        case ASTComBlockExprNode::COM_BLOCK_BLOCK_EXPR: {
          auto *blockBlockExpr = static_cast<ASTComBlockBlockExprNode *>(comBlock.get());
          checkDataTypeCompatibilityStmtList(blockBlockExpr->ifBlock->stmtList.get());
          continue;
        }
        default:
          throw std::runtime_error("Got unknown ComBlock object");
        }
      }
    }
  }
}

void Analyzer::checkDataTypeCompatibilityStmtList(ASTStmtListNode *stmtLst) {
  // Loop through statements
  for (const std::unique_ptr<ASTStmtNode> &stmt : stmtLst->stmts) {
    if (stmt->type == ASTCompStmtNode::COMP_STMT_EXPR) {
      auto *compStmt = static_cast<ASTCompStmtNode *>(stmt.get());
      checkDataTypeCompatibilityCompStmt(compStmt);
    }
  }
}

void Analyzer::checkDataTypeCompatibilityCompStmt(ASTCompStmtNode *compStmt) {
  // Abort check successfully when the key does not exist. Non-existent keys will be evaluated to false by the interpreter
  if (!jsonParser.jsonKeyExists(compStmt->key))
    return;
  // Check if 'value' has the same type as the JSON key value
  auto jsonKeyValue = jsonParser.getJSONValueFromKey(compStmt->key);

  switch (compStmt->value->type) {
  case ASTValueExprNode::STRING_EXPR:
    if (!jsonKeyValue.is_string())
      throw IncompatibleTypesException(jsonKeyValue.dump(), "a string");
    return;
  case ASTValueExprNode::BOOLEAN_EXPR:
    if (!jsonKeyValue.is_boolean())
      throw IncompatibleTypesException(jsonKeyValue.dump(), "a boolean");
    return;
  case ASTValueExprNode::NUMBER_EXPR:
    if (!jsonKeyValue.is_number_integer())
      throw IncompatibleTypesException(jsonKeyValue.dump(), "an integer");
    return;
  case ASTValueExprNode::VALUE_EXPR:
    throw UnknownDataTypeException(jsonKeyValue.dump());
  }
}