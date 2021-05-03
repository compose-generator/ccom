//
// Created by Marc on 02.05.2021.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include <string>

class Token {
private:
    int type{};
    std::string value;
    int lineNum;
    int colNum;
public:
    Token();
    Token(int, int, int);
    Token(int, std::string, int, int);
    int getType();
    std::string getValue();
    std::string getCodePos();
};

#endif //COMPILER_TOKEN_H
