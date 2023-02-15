/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>

enum TokenType {
  TOK_INVALID,

  // End of file
  TOK_EOF,

  // Keywords
  TOK_IF,       // if
  TOK_HAS,      // has
  TOK_NOT,      // not
  TOK_CONTAINS, // contains

  // Boolean values
  TOK_TRUE,  // true
  TOK_FALSE, // false

  // Operators
  TOK_OR,            // |
  TOK_EQUALS,        // ==
  TOK_NOT_EQUALS,    // !=
  TOK_GREATER,       // >
  TOK_LESS,          // <
  TOK_GREATER_EQUAL, // >=
  TOK_LESS_EQUAL,    // <=

  // Misc
  TOK_IDENTIFIER,  // e.g. test
  TOK_NUMBER,      // e.g. 123
  TOK_STRING,      // "test"
  TOK_DOT,         // .
  TOK_BRACE_OPEN,  // {
  TOK_BRACE_CLOSE, // }
  TOK_INDEX,       // [123]

  // The following examples refer to Java
  TOK_COM_IDEN_PAYLOAD,     // //
  TOK_COM_LINE_IDEN,        // //?
  TOK_COM_BLOCK_IDEN_OPEN,  // /*?
  TOK_COM_BLOCK_IDEN_CLOSE, // */

  TOK_ARBITRARY // e.g. asd'!?fowen7a_=sdfkh%"
};

class Token {
public:
  // Constructors
  Token() = default;
  Token(TokenType type, unsigned int lineNum, unsigned int colNum);
  Token(TokenType, std::string, unsigned int lineNum, unsigned int colNum);

  // Public methods
  [[nodiscard]] TokenType getType() const;
  std::string getValue();
  [[nodiscard]] std::string getCodePos() const;

private:
  // Members
  TokenType type = TOK_INVALID;
  std::string value;
  unsigned int lineNum = 0;
  unsigned int colNum = 0;
};