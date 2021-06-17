// Copyright (c) 2021 Compose Generator Contributors. All rights reserved.

#include "Reader.h"

Reader::Reader(std::string fileInput, unsigned int maxLookahead):
    fileInput(std::move(fileInput)), maxLookahead(maxLookahead) {
    // Check maxLookahead validity
    if (maxLookahead < 1) throw MaxLookaheadException(maxLookahead);

    // Load first char into buffer
    advance();

    // Set colNum to 1, if the advance was not successful due to an empty fileInput
    if (colNum == 0) colNum = 1;
}

void Reader::advance() {
    // Return EOF, when string ends
    if (inputStringPos >= fileInput.length()) {
        curChar = EOF;
        return;
    }
    // Last char was a line break?
    if (curChar == '\n') {
        lineNum++;
        colNum = 0;
    }
    // Go to next char
    curChar = (unsigned char) fileInput[inputStringPos];
    inputStringPos++;
    colNum++;
}

void Reader::expect(int input) {
    if (curChar != input)
        throw UnexpectedCharException((char) input, (char) curChar, lineNum, colNum);
    advance();
}

void Reader::expectMultiple(const std::string& input) {
    for (char const &c: input) expect(c);
}

int Reader::getLookahead() const {
    return curChar;
}

std::string Reader::getLookaheadMultiple() {
    // inputStringPos represents the position to which we have read to. e.g.: "Test", inputStringPos: 2, we have read "Te"
    int length = std::min((int) fileInput.length() - inputStringPos + 1, (int) maxLookahead);
    return fileInput.substr(inputStringPos -1, length);
}

unsigned int Reader::getLineNum() const {
    return lineNum;
}

unsigned int Reader::getColNum() const {
    return colNum;
}