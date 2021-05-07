// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <string>
#include <iostream>
#include "../lexer/Token.h"
#include "../lexer/lexer.h"


Token getNextToken();

void initParser(bool, const std::string&, const std::string&, const std::string&,
                const std::string&, const std::string&);

#endif //COMPILER_PARSER_H