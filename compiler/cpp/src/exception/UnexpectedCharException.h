//
// Created by Marc on 23.05.2021.
//

#pragma once

#include <exception>
#include <string>

class UnexpectedCharException : public std::exception {
private:
    std::string errorMessage {};
public:
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

    const char * what() const noexcept override;
};
