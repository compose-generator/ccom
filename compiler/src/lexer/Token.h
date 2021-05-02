//
// Created by Marc on 02.05.2021.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include <string>

class Token {
private:
    int type{};
    std::string stringValue;
    int intValue{};
public:
    Token();
    explicit Token(int);
    Token(int, int);
    Token(int, std::string);
    int getType();
    std::string getStringValue();
    int getIntValue();
};

#endif //COMPILER_TOKEN_H
