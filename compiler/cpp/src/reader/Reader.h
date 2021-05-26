//
// Created by Marc on 23.05.2021.
//

#pragma once

#include <string>
#include <utility>
#include <iostream>
#include "../exception/UnexpectedCharException.h"

class Reader {
private:
    // Constant inputs
    std::string fileInput;
    unsigned int maxLookahead;

    // Working variables
    int curChar = 0;
    int inputStringPos = 0;
    unsigned int lineNum = 1;
    unsigned int colNum = 0;
public:
    explicit Reader(): maxLookahead(0) {};
    Reader(std::string fileInput, unsigned int maxLookahead);

    void advance();
    void expect(int);
    void expectMultiple(const std::string&);
    int getLookahead() const;
    std::string getMaxLookahead();
    unsigned int getLineNum() const;
    unsigned int getColNum() const;
};
