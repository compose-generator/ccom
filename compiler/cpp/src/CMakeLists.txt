set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES
        main.cpp
        main.h
        util/JSONParser.cpp
        util/JSONParser.h
        reader/Reader.cpp
        reader/Reader.h
        lexer/Lexer.cpp
        lexer/Lexer.h
        lexer/Token.cpp
        lexer/Token.h
        parser/Parser.cpp
        parser/Parser.h
        parser/ast/TopLevelExprAST.cpp
        parser/ast/TopLevelExprAST.h
        parser/ast/NumberExprAST.cpp
        parser/ast/NumberExprAST.h
        parser/ast/StringExprAST.cpp
        parser/ast/StringExprAST.h
        parser/ast/IdentifierExprAST.cpp
        parser/ast/IdentifierExprAST.h
        parser/ast/StmtExprAST.cpp
        parser/ast/StmtExprAST.h
        parser/ast/StmtLstExprAST.cpp
        parser/ast/StmtLstExprAST.h
        parser/ast/KeyExprAST.cpp
        parser/ast/KeyExprAST.h
        parser/ast/IfBlockExprAST.cpp
        parser/ast/IfBlockExprAST.h
        parser/ast/PayloadExprAST.cpp
        parser/ast/PayloadExprAST.h
        parser/ast/HasStmtExprAST.cpp
        parser/ast/HasStmtExprAST.h
        parser/ast/CompStmtExprAST.cpp
        parser/ast/CompStmtExprAST.h
        parser/ast/ValueExprAST.cpp
        parser/ast/ValueExprAST.h
        parser/ast/ComBlockBlockExprAST.cpp
        parser/ast/ComBlockBlockExprAST.h
        parser/ast/ComLineBlockExprAST.cpp
        parser/ast/ComLineBlockExprAST.h
        parser/ast/SectionExprAST.cpp
        parser/ast/SectionExprAST.h
        parser/ast/ComBlockExprAST.cpp
        parser/ast/ComBlockExprAST.h
        parser/ast/ContentExprAST.cpp
        parser/ast/ContentExprAST.h
        parser/ast/ContentBlockExprAST.cpp
        parser/ast/ContentBlockExprAST.h
        parser/ast/ArbitraryExprAST.cpp
        parser/ast/ArbitraryExprAST.h
        parser/ast/BooleanExprAST.cpp
        parser/ast/BooleanExprAST.h
        analyzer/Analyzer.cpp
        analyzer/Analyzer.h
        interpreter/Interpreter.cpp
        interpreter/Interpreter.h
        exception/UnexpectedCharException.cpp
        exception/UnexpectedCharException.h
        exception/MaxLookaheadException.cpp
        exception/MaxLookaheadException.h)

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})