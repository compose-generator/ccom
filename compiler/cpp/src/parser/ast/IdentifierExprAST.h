/*
Copyright © 2021-2022 Compose Generator Contributors
All rights reserved.
*/

#pragma once

#include <string>
#include <utility>

class IdentifierExprAST {
public:
    // Constructors
    explicit IdentifierExprAST(std::string Name): name(std::move(Name)) {}
    IdentifierExprAST(std::string name, int index): name(std::move(name)), index(index) {}

    // Public methods
    std::string getName() const;
    int getIndex() const;
    std::string serialize() const;
private:
    // Members
    std::string name;
    int index = -1;
};
