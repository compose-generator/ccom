/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>

class IncompatibleTypesException : public std::exception {
public:
  // Constructors
  IncompatibleTypesException(const std::string &valueDump, const std::string &expectedTypeName) {
    errorMessage = "Incompatible data types: '" + valueDump + "' is not " + expectedTypeName;
  }

  // Public methods
  [[nodiscard]] const char *what() const noexcept override { return errorMessage.c_str(); }

private:
  // Members
  std::string errorMessage{};
};