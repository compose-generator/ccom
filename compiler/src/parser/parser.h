// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include "../lexer/Token.h"

Token getNextToken();

void initParser(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

#endif //COMPILER_PARSER_H