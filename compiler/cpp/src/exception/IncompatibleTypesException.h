#pragma once

#include <string>

class IncompatibleTypesException : public std::exception {
public:
    // Constructors
    explicit IncompatibleTypesException(const std::string& valueDump, const std::string& expectedTypeName) {
        errorMessage = "Incompatible data types: '" + valueDump + "' is not " + expectedTypeName;
    }

    // Public methods
    const char * what() const noexcept override;
private:
    // Members
    std::string errorMessage {};
};
