/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <lexer/Token.h>
#include <string>

class UnexpectedTokenException : public std::exception {
public:
  // Constructors
  UnexpectedTokenException(int expectedTokenType, int actualTokenType, const unsigned int lineNum, const unsigned int colNum) {
    errorMessage = "Expected " + getNameFromTokenType(expectedTokenType) + " but got " + getNameFromTokenType(actualTokenType) +
                   " at L" + std::to_string(lineNum) + " C" + std::to_string(colNum);
  }

  // Public methods
  [[nodiscard]] const char *what() const noexcept override { return errorMessage.c_str(); }

private:
  // Private methods
  static std::string getNameFromTokenType(int);

  // Members
  std::string errorMessage{};
};