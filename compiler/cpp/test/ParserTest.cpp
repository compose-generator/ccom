/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#include <gtest/gtest.h>

#include <parser/Parser.h>

#include "util/FileReader.h"
#include "util/LanguageDescriptor.h"

#include "../lib/json/json.hpp"

using json = nlohmann::json;

struct ParserParams {
  const std::string fileName;
};

const ParserParams PARSER_TEST_PARAMETERS[] = {{"has-block"}, {"comp-block"}, {"contains-block"}};

class ParserTests : public ::testing::TestWithParam<ParserParams> {};

TEST_P(ParserTests, TestParserWithExpectToken) {
  ParserParams param = GetParam();
  // Load parser input and expected output
  FileReader reader = FileReader("test-files");
  std::string input = reader.fileToString("parser/" + param.fileName, "parser-input.cpp");
  std::string expectedAst = reader.fileToString("parser/" + param.fileName, "expected-ast.txt");
  // Make expected AST transformations
  size_t pos;
  while ((pos = expectedAst.find("\\n")) != std::string::npos)
    expectedAst.replace(pos, 2, "\n");
  // Initialize parser
  Parser parser = Parser(false, input, "//", "/*", "*/");
  // Parse AST from input
  ASTRootNode *actualAst = parser.parseAST();
  // Serialize actual AST and compare it with the expected AST
  std::string serializedAst = actualAst->serialize();
  EXPECT_EQ(serializedAst, expectedAst);
}

INSTANTIATE_TEST_SUITE_P(ParserTests, ParserTests, ::testing::ValuesIn(PARSER_TEST_PARAMETERS));