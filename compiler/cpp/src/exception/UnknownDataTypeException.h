#pragma once

#include <string>

class UnknownDataTypeException : public std::exception {
public:
    // Constructors
    explicit UnknownDataTypeException(const std::string& valueDump) {
        errorMessage = "Unknown data type: '" + valueDump;
    }

    // Public methods
    const char * what() const noexcept override;
private:
    // Members
    std::string errorMessage {};
};
