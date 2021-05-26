//
// Created by Marc on 27.05.2021.
//

#pragma once

#include <string>

class MaxLookaheadException : public std::exception {
private:
    std::string errorMessage {};
public:
    explicit MaxLookaheadException(unsigned int maxLookahead) {
        errorMessage = "Invalid maximum lookahead '" + std::to_string(maxLookahead) + "' specified. Must be >= 1";
    }

    const char * what() const noexcept override;
};
