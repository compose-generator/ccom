// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#pragma once

#include <string>

class IncompatibleTypesException : public std::exception {
public:
    // Constructors
    explicit IncompatibleTypesException(const std::string& valueDump, const std::string& expectedTypeName) {
        errorMessage = "Incompatible data types: '" + valueDump + "' is not a " + expectedTypeName;
    }

    // Public methods
    const char * what() const noexcept override;
private:
    // Members
    std::string errorMessage {};
};
