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
#include "ast/ArbitraryExprAST.h"
#include "ast/NumberExprAST.h"
#include "ast/StringExprAST.h"
#include "ast/IdentifierExprAST.h"
#include "ast/StmtLstExprAST.h"
#include "ast/KeyExprAST.h"
#include "ast/PayloadExprAST.h"
#include "ast/IfExprAST.h"
#include "ast/CompStmtExprAST.h"
#include "ast/HasStmtExprAST.h"
#include "ast/ComBlockBlockExprAST.h"
#include "ast/ComLineBlockExprAST.h"
#include "ast/SectionExprAST.h"
#include "ast/ContentExprAST.h"
#include "../lexer/Token.h"
#include "../lexer/lexer.h"

Token getNextToken();

void initParser(bool, const std::string&, const std::string&, const std::string&,
                const std::string&, const std::string&);

std::unique_ptr<ArbitraryExprAST> parseArbitrary();
std::unique_ptr<NumberExprAST> parseNumber();
std::unique_ptr<StringExprAST> parseString();
std::unique_ptr<ValueExprAST> parseValue();
std::unique_ptr<IdentifierExprAST> parseIdentifier();
std::unique_ptr<KeyExprAST> parseKey();
std::unique_ptr<CompStmtExprAST> parseCompStmt();
std::unique_ptr<HasStmtExprAST> parseHasStmt();
std::unique_ptr<StmtExprAST> parseStmt();
std::unique_ptr<StmtLstExprAST> parseStmtList();
std::unique_ptr<PayloadExprAST> parsePayload();
std::unique_ptr<IfExprAST> parseIfBlock();
std::unique_ptr<ComBlockBlockExprAST> parseComBlockBlock();
std::unique_ptr<ComLineBlockExprAST> parseComLineBlock();
std::unique_ptr<SectionExprAST> parseSection();
std::unique_ptr<ContentExprAST> parseContent();

#endif //COMPILER_PARSER_H