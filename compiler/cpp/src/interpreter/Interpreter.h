/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include "../analyzer/Analyzer.h"
#include "../exception/UnexpectedDataTypeException.h"
#include <stdexcept>

class Interpreter {
public:
  // Constructors
  Interpreter() = delete;
  Interpreter(bool, const std::string &, const std::string &, const std::string &, const std::string &, const std::string &);

  // Public methods
  std::string interpretInput();

private:
  // Private methods
  std::string getOutput();
  std::string getOutputOfContent(ASTContentExprNode *);
  static std::string getOutputOfArbitrarySection(ASTArbitraryExprNode *);
  std::string getOutputOfRelevantSection(ASTSectionExprNode *);
  bool evaluateStmtList(ASTStmtListNode *);
  bool evaluateHasStatement(ASTHasStmtNode *);
  bool evaluateCompStatement(ASTCompStmtNode *);
  bool evaluateContainsStatement(ASTContainsStmtNode *);
  bool compareJsonWithValue(json &, ASTValueExprNode *, Operator);
  template <typename T> bool evaluateCondition(T, T, Operator);
  static bool hasSuffix(const std::string &, const std::string &);

  // Members
  Analyzer analyzer;
  JSONParser jsonParser;
  ASTRootNode *ast = nullptr;
  bool isSingleStatement = false;
};