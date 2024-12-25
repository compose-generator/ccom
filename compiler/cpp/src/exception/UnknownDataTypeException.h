/*
Copyright © 2021-2023 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>

class UnknownDataTypeException : public std::exception {
public:
  // Constructors
  explicit UnknownDataTypeException(const std::string &valueDump) { errorMessage = "Unknown data type: '" + valueDump; }

  // Public methods
  [[nodiscard]] const char *what() const noexcept override { return errorMessage.c_str(); }

private:
  // Members
  std::string errorMessage{};
};