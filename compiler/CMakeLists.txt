cmake_minimum_required(VERSION 3.19)
project(compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(compiler
        src/lexer/lexer.cpp src/lexer/lexer.h src/parser/parser.cpp src/parser/parser.h src/main.cpp src/main.h src/lexer/Token.cpp src/lexer/Token.h src/parser/ast/ExprAST.cpp src/parser/ast/ExprAST.h src/parser/ast/NumberExprAST.cpp src/parser/ast/NumberExprAST.h src/parser/ast/StringExprAST.cpp src/parser/ast/StringExprAST.h src/parser/ast/BinaryExprAST.cpp src/parser/ast/BinaryExprAST.h src/parser/ast/IdentifierExprAST.cpp src/parser/ast/IdentifierExprAST.h src/parser/ast/StmtExprAST.cpp src/parser/ast/StmtExprAST.h src/parser/ast/StmtLstExprAST.cpp src/parser/ast/StmtLstExprAST.h src/parser/ast/KeyExprAST.cpp src/parser/ast/KeyExprAST.h src/parser/ast/IfExprAST.cpp src/parser/ast/IfExprAST.h src/parser/ast/PayloadExprAST.cpp src/parser/ast/PayloadExprAST.h src/parser/ast/HasStmtExprAST.cpp src/parser/ast/HasStmtExprAST.h)
