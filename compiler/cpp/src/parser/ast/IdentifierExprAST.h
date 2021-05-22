// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 04.05.2021.
//

#pragma once

#include <string>
#include <utility>

class IdentifierExprAST {
private:
    std::string name;
    int index = -1;
public:
    explicit IdentifierExprAST(std::string Name): name(std::move(Name)) {}
    IdentifierExprAST(std::string name, int index): name(std::move(name)), index(index) {}
    std::string GetName() const;
    int GetIndex() const;
};
