//
// Created by Marc on 02.05.2021.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "../lexer/Token.h"

Token getNextToken();

void initParser(std::string, std::string, const std::string&, const std::string&, std::string);

#endif //COMPILER_PARSER_H