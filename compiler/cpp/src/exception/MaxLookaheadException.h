/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>

class MaxLookaheadException : public std::exception {
public:
  // Constructors
  explicit MaxLookaheadException(unsigned int maxLookahead) {
    errorMessage = "Invalid maximum lookahead '" + std::to_string(maxLookahead) + "' specified. Must be >= 1";
  }

  // Public methods
  [[nodiscard]] const char *what() const noexcept override { return errorMessage.c_str(); }

private:
  // Members
  std::string errorMessage{};
};