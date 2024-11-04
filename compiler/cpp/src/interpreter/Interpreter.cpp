/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#include "Interpreter.h"

Interpreter::Interpreter(bool isSingleStatement, const std::string &fileInput, const std::string &dataInput,
                         const std::string &inputCommentLineIdentifiers, const std::string &inputCommentBlockOpenIdentifiers,
                         const std::string &inputCommentBlockCloseIdentifiers) {
  this->isSingleStatement = isSingleStatement;

  // Parse input string from JSON to object
  jsonParser = JSONParser(dataInput);

  // Initialize analyzer
  analyzer = Analyzer(isSingleStatement, fileInput, jsonParser, inputCommentLineIdentifiers, inputCommentBlockOpenIdentifiers,
                      inputCommentBlockCloseIdentifiers);
  ast = analyzer.ast;

  // Execute semantic analysis
  analyzer.executeAnalysis();
}

std::string Interpreter::interpretInput() { return getOutput(); }

std::string Interpreter::getOutput() {
  if (isSingleStatement) {
    if (ast->type != ASTRootNode::STMT_LST_EXPR)
      throw std::runtime_error("Input was no single statement list");
    auto *stmtLst = static_cast<ASTStmtListNode *>(ast);
    return evaluateStmtList(stmtLst) ? "true" : "false";
  } else {
    auto *content = static_cast<ASTContentExprNode *>(ast);
    return getOutputOfContent(content);
  }
}

std::string Interpreter::getOutputOfContent(ASTContentExprNode *content) {
  std::string result;

  for (const std::unique_ptr<ASTContentBlockExprNode> &contentBlock : content->contentBlocks) {
    if (contentBlock->type == ASTContentBlockExprNode::ARBITRARY_EXPR) { // Is section an arbitrary section?
      auto *arbitrarySection = static_cast<ASTArbitraryExprNode *>(contentBlock.get());
      result += getOutputOfArbitrarySection(arbitrarySection);
    } else if (contentBlock->type == ASTContentBlockExprNode::SECTION_EXPR) { // Is section a relevant section?
      auto *sectionExpr = static_cast<ASTSectionExprNode *>(contentBlock.get());
      result += getOutputOfRelevantSection(sectionExpr);
    }
  }
  return result;
}

std::string Interpreter::getOutputOfArbitrarySection(ASTArbitraryExprNode *arbitraryExpr) {
  std::string arbitrary = arbitraryExpr->value;
  // Cut off first char, if it is a line break
  if (arbitrary.rfind('\n', 0) == 0)
    arbitrary.erase(0, 1);
  return arbitrary;
}

std::string Interpreter::getOutputOfRelevantSection(ASTSectionExprNode *relevantSection) {
  std::string result;

  // Loop through com blocks
  for (const std::unique_ptr<ASTComBlockExprNode> &comBlock : relevantSection->comBlocks) {
    switch (comBlock->type) {
    case ASTComBlockExprNode::COM_LINE_BLOCK_EXPR: {
      auto *lineBlockExpr = static_cast<ASTComLineBlockExprNode *>(comBlock.get());
      const std::unique_ptr<ASTIfBlockExprNode> &ifBlock = lineBlockExpr->ifBlock;
      // Evaluate condition and append payload to output string if condition was truthy
      if (evaluateStmtList(ifBlock->stmtList.get())) {
        result += ifBlock->payload->value;
        if (!hasSuffix(result, "\n"))
          result += "\n";
      }
      break;
    }
    case ASTComBlockExprNode::COM_BLOCK_BLOCK_EXPR: {
      auto *blockBlockExpr = static_cast<ASTComBlockBlockExprNode *>(comBlock.get());
      const std::unique_ptr<ASTIfBlockExprNode> &ifBlock = blockBlockExpr->ifBlock;
      // Evaluate condition and append payload to output string if condition was truthy
      if (evaluateStmtList(ifBlock->stmtList.get())) {
        result += ifBlock->payload->value;
      }
      break;
    }
    default:
      throw std::runtime_error("Got unknown ComBlock object");
    }
  }
  return result;
}

bool Interpreter::evaluateStmtList(ASTStmtListNode *stmtList) {
  // Loop through statements
  for (const std::unique_ptr<ASTStmtNode> &stmt : stmtList->stmts) {
    switch (stmt->type) {
    case ASTStmtNode::HAS_STMT_EXPR: {
      auto *hasStmt = static_cast<ASTHasStmtNode *>(stmt.get());
      if (evaluateHasStatement(hasStmt))
        return true;
      continue;
    }
    case ASTStmtNode::COMP_STMT_EXPR: {
      auto *compStmt = static_cast<ASTCompStmtNode *>(stmt.get());
      if (evaluateCompStatement(compStmt))
        return true;
      continue;
    }
    case ASTStmtNode::CONTAINS_STMT_EXPR: {
      auto *containsStmt = static_cast<ASTContainsStmtNode *>(stmt.get());
      if (evaluateContainsStatement(containsStmt))
        return true;
      continue;
    }
    default:
      throw std::runtime_error("Got unknown Stmt object");
    }
  }
  return false;
}

bool Interpreter::evaluateHasStatement(ASTHasStmtNode *hasStmt) {
  bool isKeyExisting = jsonParser.jsonKeyExists(hasStmt->key);
  if (hasStmt->isInverted)
    return !isKeyExisting;
  return isKeyExisting;
}

bool Interpreter::evaluateCompStatement(ASTCompStmtNode *compStmt) {
  if (!jsonParser.jsonKeyExists(compStmt->key))
    return false;
  json keyValue = jsonParser.getJSONValueFromKey(compStmt->key);
  return compareJsonWithValue(keyValue, compStmt->value.get(), compStmt->op);
}

bool Interpreter::evaluateContainsStatement(ASTContainsStmtNode *containsStmt) {
  if (!jsonParser.jsonKeyExists(containsStmt->listKey))
    return false;
  json listKeyValue = jsonParser.getJSONValueFromKey(containsStmt->listKey);

  const Operator op = containsStmt->op;

  if (listKeyValue.is_array()) {
    // Loop through array
    for (auto &listItem : listKeyValue) {
      // Skip the comparison, if the sub-key does not even exist
      if (!JSONParser::jsonKeyExists(listItem, containsStmt->valueKey))
        continue;
      // Read value
      json valueKeyValue = JSONParser::getJSONValueFromKey(listItem, containsStmt->valueKey);
      // Compare data with hardcoded value
      if (compareJsonWithValue(valueKeyValue, containsStmt->value.get(), op))
        return !containsStmt->isInverted;
    }
    return containsStmt->isInverted;
  } else if (listKeyValue.is_object()) {
    // Cancel the comparison, if the sub-key does not even exist
    if (!JSONParser::jsonKeyExists(listKeyValue, containsStmt->valueKey))
      return containsStmt->isInverted;
    // Read value
    json valueKeyValue = JSONParser::getJSONValueFromKey(listKeyValue, containsStmt->valueKey);
    // Compare data with hardcoded value
    if (compareJsonWithValue(valueKeyValue, containsStmt->value.get(), op))
      return !containsStmt->isInverted;
    return containsStmt->isInverted;
  }
  throw UnexpectedDataTypeException(listKeyValue.dump(), "array or object");
}

bool Interpreter::compareJsonWithValue(json &keyValue, ASTValueExprNode *value, Operator op) {
  if (keyValue.is_string()) {
    auto leftValue = keyValue.get<std::string>();
    if (value->type == ASTValueExprNode::STRING_EXPR) {
      auto *rightValue = static_cast<ASTStringExprNode *>(value);
      return evaluateCondition(leftValue, rightValue->value, op);
    }
    // This should never get triggered, because invalid type combinations are already filtered out
    throw std::runtime_error("Internal compiler error - left value was string and right was not");
  } else if (keyValue.is_boolean()) {
    auto leftValue = keyValue.get<bool>();
    if (value->type == ASTValueExprNode::BOOLEAN_EXPR) {
      auto *rightValue = static_cast<ASTBooleanExprNode *>(value);
      return evaluateCondition(leftValue, rightValue->value, op);
    }
    // This should never get triggered, because invalid type combinations are already filtered out
    throw std::runtime_error("Internal compiler error - left value was boolean and right was not");
  } else if (keyValue.is_number_integer()) {
    auto leftValue = keyValue.get<int>();
    if (value->type == ASTValueExprNode::NUMBER_EXPR) {
      auto *rightValue = static_cast<ASTNumberExprNode *>(value);
      return evaluateCondition(leftValue, rightValue->value, op);
    }
    // This should never get triggered, because invalid type combinations are already filtered out
    throw std::runtime_error("Internal compiler error - left value was int and right was not");
  }
  throw std::runtime_error("Unknown datatype of '" + keyValue.dump() + "'");
}

template <typename T> bool Interpreter::evaluateCondition(T leftValue, T rightValue, Operator op) {
  switch (op) {
  case OP_EQUALS:
    return leftValue == rightValue;
  case OP_NOT_EQUALS:
    return leftValue != rightValue;
  case OP_GREATER:
    return leftValue > rightValue;
  case OP_LESS:
    return leftValue < rightValue;
  case OP_GREATER_EQUAL:
    return leftValue >= rightValue;
  case OP_LESS_EQUAL:
    return leftValue <= rightValue;
  }
  return false;
}

bool Interpreter::hasSuffix(const std::string &str, const std::string &suffix) {
  return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}