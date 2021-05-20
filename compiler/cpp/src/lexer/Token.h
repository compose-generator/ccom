// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#pragma once

#include <string>

class Token {
private:
    int type = 0;
    std::string value;
    unsigned int lineNum = 0;
    unsigned int colNum = 0;
public:
    Token();
    Token(int, unsigned int, unsigned int);
    Token(int, std::string, unsigned int, unsigned int);
    int getType() const;
    std::string getValue();
    std::string getCodePos();
};