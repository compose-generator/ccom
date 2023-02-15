set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES
        main.cpp
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
        parser/ASTNodes.h
        analyzer/Analyzer.cpp
        analyzer/Analyzer.h
        interpreter/Interpreter.cpp
        interpreter/Interpreter.h
        exception/UnexpectedCharException.h
        exception/MaxLookaheadException.h
        exception/UnexpectedTokenException.cpp
        exception/UnexpectedTokenException.h
        exception/IncompatibleTypesException.h
        exception/UnknownDataTypeException.h
        exception/UnexpectedDataTypeException.h)

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES} parser/ASTNodes.cpp)