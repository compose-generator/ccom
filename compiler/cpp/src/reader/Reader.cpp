//
// Created by Marc on 23.05.2021.
//

#include "Reader.h"

Reader::Reader(std::string fileInput, unsigned int maxLookahead):
    fileInput(std::move(fileInput)), maxLookahead(maxLookahead) {
    advance(); // Load first char into buffer
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
    if (curChar != input) throw UnexpectedCharException((char) input, (char) curChar, lineNum, colNum);
    advance();
}

void Reader::expectMultiple(const std::string& input) {
    for (char const &c: input) expect(c);
}

int Reader::getLookahead() const {
    return curChar;
}

std::string Reader::getMaxLookahead() {
    int length = std::min((int) fileInput.length() - inputStringPos, (int) maxLookahead);
    return fileInput.substr(inputStringPos -1, length);
}

unsigned int Reader::getLineNum() const {
    return lineNum;
}

unsigned int Reader::getColNum() const {
    return colNum;
}