#pragma once

#include <string>
#include <utility>
#include "ContentBlockExprAST.h"

class ArbitraryExprAST : public ContentBlockExprAST {
public:
    // Constructors
    explicit ArbitraryExprAST(std::string value): ContentBlockExprAST(ContentBlockExprType::ARBITRARY_EXPR),
        value(std::move(value)) {}

    // Public methods
    std::string getValue() const;
    std::string serialize() const;
private:
    // Members
    std::string value;
};
