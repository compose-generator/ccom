//
// Created by Marc on 23.05.2021.
//

#pragma once

#include <string>
#include <utility>
#include <iostream>
#include "../exception/UnexpectedCharException.h"
#include "../exception/MaxLookaheadException.h"

class Reader {
private:
    // Constant inputs
    std::string fileInput;
    unsigned int maxLookahead;

    // Working variables
    int curChar = 0;
    int inputStringPos = 0; // Represents the position to which we have read to. e.g.: "Test", inputStringPos: 2, we have read "Te"
    unsigned int lineNum = 1;
    unsigned int colNum = 0;
public:
    explicit Reader(): maxLookahead(0) {};
    Reader(std::string fileInput, unsigned int maxLookahead);

    void advance();
    void expect(int);
    void expectMultiple(const std::string&);
    int getLookahead() const;
    std::string getLookaheadMultiple();
    unsigned int getLineNum() const;
    unsigned int getColNum() const;
};