/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>
#include <utility>
#include <iostream>
#include "../exception/UnexpectedCharException.h"
#include "../exception/MaxLookaheadException.h"

class Reader {
public:
  // Constructors
  explicit Reader() : maxLookahead(0){};
  Reader(std::string fileInput, unsigned int maxLookahead);

  // Public methods
  void advance();
  void expect(int);
  void expectMultiple(const std::string &);
  [[nodiscard]] int getLookahead() const;
  std::string getLookaheadMultiple();
  [[nodiscard]] unsigned int getLineNum() const;
  [[nodiscard]] unsigned int getColNum() const;

private:
  // Members
  std::string fileInput;
  unsigned int maxLookahead;
  int curChar = 0;
  int inputStringPos = 0; // Represents the position to which we have read to. e.g.: "Test", inputStringPos: 2, we have read "Te"
  unsigned int lineNum = 1;
  unsigned int colNum = 0;
};