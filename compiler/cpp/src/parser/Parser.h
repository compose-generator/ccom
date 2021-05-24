//
// Created by Marc on 24.05.2021.
//

# pragma once

#include <memory>
#include "../lexer/Lexer.h"
#include "ast/TopLevelExprAST.h"
#include "ast/ArbitraryExprAST.h"
#include "ast/NumberExprAST.h"
#include "ast/BooleanExprAST.h"
#include "ast/StringExprAST.h"
#include "ast/IdentifierExprAST.h"
#include "ast/StmtExprAST.h"
#include "ast/StmtLstExprAST.h"
#include "ast/KeyExprAST.h"
#include "ast/PayloadExprAST.h"
#include "ast/IfBlockExprAST.h"
#include "ast/CompStmtExprAST.h"
#include "ast/HasStmtExprAST.h"
#include "ast/ComBlockBlockExprAST.h"
#include "ast/ComLineBlockExprAST.h"
#include "ast/SectionExprAST.h"
#include "ast/ContentExprAST.h"

class Parser {
private:
    // Constant inputs
    bool isSingleStatement;

    // Working variables
    Lexer lexer;

    // Private functions
    std::unique_ptr<ContentExprAST> parseContent();
    std::unique_ptr<ArbitraryExprAST> parseArbitrary();
    std::unique_ptr<PayloadExprAST> parsePayload();
    std::unique_ptr<SectionExprAST> parseSection();
    std::unique_ptr<StmtLstExprAST> parseStmtList();
    std::unique_ptr<StmtExprAST> parseStmt();
    std::unique_ptr<HasStmtExprAST> parseHasStmt();
    std::unique_ptr<CompStmtExprAST> parseCompStmt();
    std::unique_ptr<IfBlockExprAST> parseIfBlock();
    std::unique_ptr<ComLineBlockExprAST> parseComLineBlock();
    std::unique_ptr<ComBlockBlockExprAST> parseComBlockBlock();
    std::unique_ptr<KeyExprAST> parseKey();
    std::unique_ptr<IdentifierExprAST> parseIdentifier();
    std::unique_ptr<ValueExprAST> parseValue();
    std::unique_ptr<StringExprAST> parseString();
    std::unique_ptr<BooleanExprAST> parseBoolean();
    std::unique_ptr<NumberExprAST> parseNumber();
public:
    explicit Parser() {}
    Parser(bool, const std::string&, const std::string&, const std::string&, const std::string&);
    TopLevelExprAST* parseAST();
};
