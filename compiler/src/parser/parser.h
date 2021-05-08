// Copyright (c) Marc Auberer 2021. All rights reserved.

//
// Created by Marc on 02.05.2021.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include "ast/ExprAST.h"
#include "ast/NumberExprAST.h"
#include "ast/StringExprAST.h"
#include "ast/IdentifierExprAST.h"
#include "ast/StmtLstExprAST.h"
#include "ast/KeyExprAST.h"
#include "../lexer/Token.h"
#include "../lexer/lexer.h"

Token getNextToken();

void initParser(bool, const std::string&, const std::string&, const std::string&,
                const std::string&, const std::string&);

std::unique_ptr<NumberExprAST> parseNumber();
std::unique_ptr<StringExprAST> parseString();
std::unique_ptr<ExprAST> parseValue();
std::unique_ptr<IdentifierExprAST> parseIdentifier();
std::unique_ptr<KeyExprAST> parseKey();
std::unique_ptr<ExprAST> parseCompStmt();
std::unique_ptr<ExprAST> parseHasStmt();
std::unique_ptr<StmtExprAST> parseStmt();
std::unique_ptr<StmtLstExprAST> parseStmtList();
std::unique_ptr<ExprAST> parsePayload();
std::unique_ptr<ExprAST> parseComBlockIdenClose();
std::unique_ptr<ExprAST> parseComBlockIdenOpen();
std::unique_ptr<ExprAST> parseComIdenPayload();
std::unique_ptr<ExprAST> parseComLineIden();
std::unique_ptr<ExprAST> parseIfBlock();
std::unique_ptr<ExprAST> parseComBlockBlock();
std::unique_ptr<ExprAST> parseComLineBlock();
std::unique_ptr<ExprAST> parseSection();
std::unique_ptr<ExprAST> parseContent();

#endif //COMPILER_PARSER_H