/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <stdexcept>
#include <string>

#include <exception/IncompatibleTypesException.h>
#include <exception/UnknownDataTypeException.h>
#include <parser/ASTNodes.h>
#include <parser/Parser.h>
#include <util/JSONParser.h>

#include "../../lib/json/json.hpp"

using json = nlohmann::json;

class Analyzer {
public:
  // Constructors
  Analyzer() = default;
  Analyzer(bool, const std::string &, JSONParser, const std::string &, const std::string &, const std::string &);

  // Public methods
  void executeAnalysis();

  // Members
  ASTRootNode *ast = nullptr;

private:
  // Private methods
  void checkDataTypeCompatibility();
  void checkDataTypeCompatibilityContent(ASTContentExprNode *);
  void checkDataTypeCompatibilityStmtList(ASTStmtListNode *);
  void checkDataTypeCompatibilityCompStmt(ASTCompStmtNode *);

  // Members
  Parser parser;
  JSONParser jsonParser;
  bool isSingleStatement = false;
};