/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <exception>
#include <string>

class UnexpectedCharException : public std::exception {
public:
    // Constructors
    UnexpectedCharException(const char unexpectedChar, const unsigned int lineNum, const unsigned int colNum) {
        errorMessage = "Unexpected character '" + std::string(1, unexpectedChar) + "' at L"+
                std::to_string(lineNum) + " C" + std::to_string(colNum);
    }

    UnexpectedCharException(const char expectedChar, const char unexpectedChar, const unsigned int lineNum,
                            const unsigned int colNum) {
        errorMessage = "Expected '" + std::string(1, expectedChar) + "', but got '" +
                std::string(1, unexpectedChar) + "' at L" + std::to_string(lineNum) + " C" +
                std::to_string(colNum);
    }

    // Public methods
    const char * what() const noexcept override;
private:
    // Members
    std::string errorMessage {};
};
